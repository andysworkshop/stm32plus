/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

namespace stm32plus {
  namespace net {

    DECLARE_EVENT_SIGNATURE(TcpAccept,void (TcpAcceptEvent&));


    /**
     * Non-template base class for the TCP server class. Most of the implementation is here. The template
     * is really only needed to manage the creation of correctly typed connections
     */

    class TcpServerBase {

      public:

        /**
         * Server parameters
         */

        struct Parameters {

          uint16_t tcp_maxConnectionsPerServer;       ///< maximum number of connections to accept per server. Default is 5
          uint32_t tcp_idleConnectionTimeout;         ///< the time in millis after which an idle connection will be auto-closed. Zero never times out. The default is zero.

          /**
           * Constructor
           */

          Parameters() {
            tcp_maxConnectionsPerServer=5;      // 5 simultaneous connections per server
            tcp_idleConnectionTimeout=0;        // never time out
          }
        };

      protected:
        uint16_t _listeningPort;                              // listener port for this server
        NetworkUtilityObjects& _networkUtilityObjects;        // various utils
        TcpEvents& _tcpEvents;                                // event source for TCP receive events
        const Parameters& _params;                            // reference to the parameters instance
        uint16_t _segmentSizeLimit;                           // maximum segment size (MSS)
        uint16_t _additionalHeaderSize;                       // additional bytes required for lower layer headers

        volatile bool _started;                               // true if the server has been started
        uint16_t _connectionCount;                            // total number of connections for this server

      public:
        TcpServerBase(uint16_t listeningPort,
                      NetworkUtilityObjects& networkUtilityObjects,
                      TcpEvents& tcpEvents,
                      const Parameters& params,
                      uint16_t segmentSizeLimit,
                      uint16_t additionalHeaderSize);
        ~TcpServerBase();

        DECLARE_EVENT_SOURCE(TcpAccept);

        void start();
        uint16_t getListeningPort() const;
        const Parameters& getParameters() const;
        NetworkUtilityObjects& getNetworkUtilityObjects() const;

      protected:
        void onNotification(NetEventDescriptor& ned);
        void handleConnectionReleased(const TcpConnectionReleasedEvent& tcre);
    };


    /**
     * Constructor
     * @param listeningPort the port number to listen on
     */

    inline TcpServerBase::TcpServerBase(uint16_t listeningPort,
                                        NetworkUtilityObjects& networkUtilityObjects,
                                        TcpEvents& tcpEvents,
                                        const Parameters& params,
                                        uint16_t segmentSizeLimit,
                                        uint16_t additionalHeaderSize)
      : _listeningPort(listeningPort),
        _networkUtilityObjects(networkUtilityObjects),
        _tcpEvents(tcpEvents),
        _params(params),
        _segmentSizeLimit(segmentSizeLimit),
        _additionalHeaderSize(additionalHeaderSize) {

      _started=false;
      _connectionCount=0;

      // subscribe to network notifications

      _networkUtilityObjects.NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&TcpServerBase::onNotification));
    }


    /**
     * Send a notification that this server is being released
     */

    inline TcpServerBase::~TcpServerBase() {

      // unsubscribe from notify events

      _networkUtilityObjects.NetworkNotificationEventSender.removeSubscriber(NetworkNotificationEventSourceSlot::bind(this,&TcpServerBase::onNotification));
    }


    /**
     * Start the server. After this method has completed the server is ready to accept connections
     */

    inline void TcpServerBase::start() {
      _started=true;
    }


    /**
     * Get the listening port
     * @return the port number
     */

    inline uint16_t TcpServerBase::getListeningPort() const {
      return _listeningPort;
    }


    /**
     * Get the net utils
     * @return The network utilities
     */

    inline NetworkUtilityObjects& TcpServerBase::getNetworkUtilityObjects() const {
      return _networkUtilityObjects;
    }


    /**
     * Network notification event
     * @param ned The event descriptor
     */

    __attribute__((noinline)) inline void TcpServerBase::onNotification(NetEventDescriptor& ned) {

      if(ned.eventType==NetEventDescriptor::NetEventType::TCP_CONNECTION_RELEASED)
        handleConnectionReleased(static_cast<TcpConnectionReleasedEvent&>(ned));
    }


    /**
     * A connection is released
     * @param tcre The connection released event
     */

    inline void TcpServerBase::handleConnectionReleased(const TcpConnectionReleasedEvent& tcre) {

      // if it was a connection to our server then release it

      if(tcre.connection.getLocalPort()==_listeningPort)
        _connectionCount--;
    }


    /**
     * Get a reference to the parameters class
     * @return A parameters reference
     */

    inline const TcpServerBase::Parameters& TcpServerBase::getParameters() const {
      return _params;
    }
  }
}
