/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Class to represent the lifecycle of a TCP connection. This is a base class and
     * will not be created directly. Users derive from this to implement the I/O functionality
     * they need for their connection.
     */

    DECLARE_EVENT_SIGNATURE(TcpConnectionClosed,void (TcpConnectionClosedEvent&));
    DECLARE_EVENT_SIGNATURE(TcpConnectionDataReady,void (TcpConnectionDataReadyEvent&));

    class TcpConnection {

      public:

      /**
       * Error codes
       */

      enum {
        E_TIMED_OUT = 1,
        E_INVALID_STATE,        ///< you tried to do something when the connection is not in a state that would allow it
        E_CONNECTION_RESET      ///< connection was closed while we were sending data
      };


      /**
       * Parameters class
       */

      struct Parameters {

        uint16_t tcp_receiveBufferSize;     ///< per-connection receive buffer size. Default is 256 bytes.
        uint32_t tcp_initialResendDelay;    ///< first delay to resend an un-acked segment. Default is 4 seconds.
        uint32_t tcp_maxResendDelay;        ///< the resend delay exponential backoff is capped at this value. default is 60 (1 minute)
        bool tcp_push;                      ///< if true, set the PSH flag in sent segments. Default is false.
        bool tcp_nagleAvoidance;            ///< if true, single packet sends are broken into 2 to force the receiver's Nagle algorithm to generate an ACK without delay. Default is true.

        /**
         * Constructor
         */

        Parameters() {
          tcp_receiveBufferSize=256;
          tcp_maxResendDelay=60000;
          tcp_initialResendDelay=4000;
          tcp_nagleAvoidance=true;
          tcp_push=false;
        }
      };

      protected:
        NetworkUtilityObjects *_networkUtilityObjects;
        TcpEvents *_tcpEvents;

        TcpReceiveBuffer *_receiveBuffer;
        uint16_t _remoteMss;
        uint16_t _segmentSizeLimit;
        uint16_t _additionalHeaderSize;
        uint32_t _lastZeroWindowPollTime;
        uint32_t _zeroWindowPollDelay;
        uint32_t _lastActiveTime;                   // from the millisecond timer
        TcpConnectionState _state;
        const Parameters& _params;
        bool _receiveWindowIsClosed;

      protected:
        void onNotification(NetEventDescriptor& ned);
        void onReceive(TcpSegmentEvent& event);

        void handleIncomingSynAck(const TcpHeader& header);
        void handleIncomingAck(const TcpHeader& header,bool hasData);
        void handleIncomingFin(TcpSegmentEvent& event);
        void handleIncomingRst();
        void handleIncomingData(const TcpSegmentEvent& event);

        void initialise(const IpAddress& remoteAddress,uint16_t remotePort,uint16_t localPort);
        void handleFindConnectionEvent(TcpFindConnectionNotificationEvent& tfcne);

        bool sendSynAck();

        uint16_t getReceiveBufferSpaceAvailable() const;
        uint16_t sillyWindowAvoidance();
        bool receiveWindowCanBeOpened() const;

      public:
        TcpConnection(const Parameters& params);
        ~TcpConnection();

        bool sendSyn();

        bool initialise(NetworkUtilityObjects& networkUtilityObjects,
                        TcpEvents& tcpEvents,
                        TcpSegmentEvent& segmentEvent,
                        uint16_t segmentSizeLimit,
                        uint16_t additionalHeaderSize);

        bool initialise(NetworkUtilityObjects& networkUtilityObjects,
                        TcpEvents& tcpEvents,
                        const IpAddress& remoteAddress,
                        uint16_t localPort,
                        uint16_t remotePort,
                        uint16_t segmentSizeLimit,
                        uint16_t additionalHeaderSize);

        uint16_t getLocalPort() const;
        const IpAddress& getRemoteAddress() const;
        const TcpConnectionState& getConnectionState() const;

        bool receive(void *data,uint32_t dataSize,uint32_t& actuallyReceived,uint32_t timeoutMillis=0);
        bool send(const void *data,uint32_t dataSize,uint32_t& actuallySent,uint32_t timeoutMillis=0);
        bool abort();

        bool isRemoteEndClosed() const;
        bool isLocalEndClosed() const;
        bool waitForStateChange(TcpState oldState,uint32_t timeoutMillis) const;

        uint16_t getTransmitWindowSize() const;
        uint16_t getDataAvailable() const;

        uint32_t getLastActiveTime() const;

        DECLARE_EVENT_SOURCE(TcpConnectionClosed);
        DECLARE_EVENT_SOURCE(TcpConnectionDataReady);
    };


    /**
     * Constructor. This actual instance referenced by 'params' lives either in the TcpServer instance
     * for TCP servers or in the TcpClientConnection instance for clients. For servers this saves memory
     * because there will be only ever one parameters instance shared between all the connections created
     * by the server.
     *
     * For servers, in your derivation of TcpConnection you can also derive a Parameters class from
     * TcpConnection::Parameters and the server will automagically use that and it will be passed to your
     * constructor. See the net_web_server example for an example where I use this method to tweak some
     * of the TcpConnection parameters.
     *
     * @param reference to the parameters class that we'll use
     */

    inline TcpConnection::TcpConnection(const Parameters& params)
      : _params(params) {
    }


    /**
     * Handle an event that's searching for a connection
     * @param tfcne The event descriptor
     */

    inline void TcpConnection::handleFindConnectionEvent(TcpFindConnectionNotificationEvent& tfcne) {

      if(tfcne.localPort==_state.localPort &&
         tfcne.remotePort==_state.remotePort &&
         tfcne.remoteAddress==_state.remoteAddress)
        tfcne.tcpConnection=this;
    }


    /**
     * Get the local port
     * @return the local port number
     */

    inline uint16_t TcpConnection::getLocalPort() const {
      return _state.localPort;
    }


    /**
     * Get the connection state object
     * @return A reference to the remote state
     */

    inline const TcpConnectionState& TcpConnection::getConnectionState() const {
      return _state;
    }


    /**
     * Wait for the state to move from the given last-known-state
     * @param oldState the state we think it's currently at
     * @param timeout, in millis, or zero to block
     */

    inline bool TcpConnection::waitForStateChange(TcpState oldState,uint32_t timeoutMillis) const {

      uint32_t now;

      now=MillisecondTimer::millis();

      while(_state.state==oldState)
        if(timeoutMillis && MillisecondTimer::hasTimedOut(now,timeoutMillis))
          return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_TIMED_OUT);

      return true;
    }


    /**
     * Return true if the remote end has closed its port. True here means that
     * the remote end will not send any more data
     * @return true if it has closed
     */

    inline bool TcpConnection::isRemoteEndClosed() const {
      return _state.state==TcpState::CLOSE_WAIT ||
             _state.state==TcpState::LAST_ACK ||
             _state.state==TcpState::CLOSING ||
             _state.state==TcpState::TIME_WAIT;
    }


    /**
     * Return true if the local end has closed its port. Closed here means
     * that no more data can be accepted for sending
     * @return true if it has closed
     */

    inline bool TcpConnection::isLocalEndClosed() const {
      return _state.state==TcpState::FIN_WAIT_1 ||
             _state.state==TcpState::FIN_WAIT_2 ||
             _state.state==TcpState::CLOSING ||
             _state.state==TcpState::TIME_WAIT ||
             _state.state==TcpState::LAST_ACK ||
             _state.state==TcpState::CLOSED;
    }


    /**
     * Get the size of the transmit window. The transmit window is the maximum number of bytes that can
     * be send to the other side without blocking. The caller can poll for this being non-zero as an indicator
     * that the remote end is ready to receive data. The actual non-zero size may not be particularly useful
     * because the remote end can continually change its window size with each segment acknowledgement.
     * @return The transmit window size
     */

    inline uint16_t TcpConnection::getTransmitWindowSize() const {
      return _state.txWindow.sendWindow;
    }


    /**
     * Get the amount of data available for reading without blocking. The maximum
     * amount that can ever be returned by this function is the value that you specified in the
     * tcp_receiveBufferSize configuration parameter.
     */

    inline uint16_t TcpConnection::getDataAvailable() const {
      return _receiveBuffer->availableToRead();
    }


    /**
     * Try to abort this connection by sending an RST to the other end.
     * @return true if it was in an abortable state and an RST has been sent
     */

    inline bool TcpConnection::abort() {

      switch(_state.state) {

        case TcpState::SYN_RCVD:
        case TcpState::SYN_SENT:
        case TcpState::ESTABLISHED:
          _state.sendRstAck(*_networkUtilityObjects,0);
          return true;

        default:
          return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_INVALID_STATE);
      }
    }


    /**
     * Get the amount of write space available in the receive buffer
     * @return The amount of space
     */

    inline uint16_t TcpConnection::getReceiveBufferSpaceAvailable() const {
      return _receiveBuffer->availableToWrite();
    }


    /**
     * Get the remote IP address
     * @return The IP address
     */

    inline const IpAddress& TcpConnection::getRemoteAddress() const {
      return _state.remoteAddress;
    }


    /**
     * Get the last active time in millis for this connection. The 'active time' is the last
     * time that send() was called or the last time that a data-carrying segment arrived.
     * @return The last time in millis.
     */

    inline uint32_t TcpConnection::getLastActiveTime() const {
      return _lastActiveTime;
    }


    /**
     * Return true if the receive window advertised in an ACK can be opened
     * @return true if the current receive window can be advertised
     */

    inline bool TcpConnection::receiveWindowCanBeOpened() const {
      return _state.rxWindow.receiveWindow>=std::min(_params.tcp_receiveBufferSize/2,(int)_segmentSizeLimit);
    }


    /**
     * Handle a RST coming from the other side.
     * Change the state to CLOSED
     */

    inline void TcpConnection::handleIncomingRst() {

      // the connection has gone bad

      _state.changeState(*_networkUtilityObjects,TcpState::CLOSED);

      // notify

      TcpConnectionClosedEventSender.raiseEvent(TcpConnectionClosedEvent(*this));
    }


    /**
     * Return an appropriate window size for an ACK based on Clarke's simple algorithm
     * for avoiding silly window syndrome.
     *
     * If the current real window is less than the lesser of half the buffer size and 1 segment
     * then we close the window, otherwise we advertise a real window size
     *
     * This is IRQ code.
     *
     * @return The window size.
     */

    inline uint16_t TcpConnection::sillyWindowAvoidance() {

      if(receiveWindowCanBeOpened()) {
        _receiveWindowIsClosed=false;
        return _state.rxWindow.receiveWindow;
      }

      _receiveWindowIsClosed=true;
      return 0;
    }
  }
}
