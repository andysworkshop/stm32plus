/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Class to manage a listening TCP server. Each TCP server has a port on which
     * it listens for incoming connections. When a remote client attempts a SYN with the
     * server we create a connection object to handle the interaction and leave it to
     * handle the state machine.
     *
     * Unlike the common berkeley sockets implementation there is no concept of a 'listen'
     * backlog because the API is entirely event based. The server is created in an unstarted
     * state. The user should subscribe to accept events and then call start().
     *
     * @tparam TConnection The user's derivation of the library-supplied connection class.
     * @tparam TUser optional type of a pointer that the user would like to be passed to the connection constructor.
     */

    template<class TConnection,class TUser=void>
    class TcpServer : public TcpServerBase {

      protected:
        TUser *_userptr;                                            // connection constructor may take optional typed user parameter
        const typename TConnection::Parameters _connectionParams;   // connection parameters are shared between instances

      protected:
        void onReceive(TcpSegmentEvent&);

      public:
        TcpServer(uint16_t listeningPort,
                  NetworkUtilityObjects& networkUtilityObjects,
                  TcpEvents& tcpEvents,
                  const Parameters& params,
                  uint16_t segmentSizeLimit,
                  uint16_t additionalHeaderSize,
                  TUser *userptr);

        ~TcpServer();
    };


    /**
     * Create a connection type by calling the correct constructor without forcing
     * the user to declare the 'other' constructor
     */

    namespace {

      /**
       * This instance creates a connection class where the user wants to pass in a
       * user-defined parameter
       */

      template<class TConnection,class TUser>
      struct ConnectionCreator {
        static TConnection *createConnection(const typename TConnection::Parameters& params,TUser *userptr) {
          return new TConnection(params,userptr);
        }
      };


      /**
       * This instance is a specialisation that calls the default constructor when the
       * user does not want to pass in a user defined parameter
       */

      template<class TConnection>
      struct ConnectionCreator<TConnection,void> {
        static TConnection *createConnection(const typename TConnection::Parameters& params,void *) {
          return new TConnection(params);
        }
      };
    }

    /**
     * Constructor
     * @param listeningPort the port number to listen on
     */

    template<class TConnection,class TUser>
    inline TcpServer<TConnection,TUser>::TcpServer(uint16_t listeningPort,
                                             NetworkUtilityObjects& networkUtilityObjects,
                                             TcpEvents& tcpEvents,
                                             const Parameters& params,
                                             uint16_t segmentSizeLimit,
                                             uint16_t additionalHeaderSize,
                                             TUser *userptr)
      : TcpServerBase(listeningPort,
                      networkUtilityObjects,
                      tcpEvents,
                      params,
                      segmentSizeLimit,
                      additionalHeaderSize),
        _userptr(userptr) {

      // subscribe to receive events

      _tcpEvents.TcpReceiveEventSender.insertSubscriber(TcpReceiveEventSourceSlot::bind(this,&TcpServer::onReceive));
    }


    /**
     * Send a notification that this server is being released
     */

    template<class TConnection,class TUser>
    inline TcpServer<TConnection,TUser>::~TcpServer() {

      // unsubscribe from receive events

      _tcpEvents.TcpReceiveEventSender.removeSubscriber(TcpReceiveEventSourceSlot::bind(this,&TcpServer::onReceive));

      // raise the event that we're going away

      _networkUtilityObjects.NetworkNotificationEventSender.raiseEvent(TcpServerReleasedEvent(*this));
    }


    /**
     * Network receive event
     * @param ned The event descriptor
     */

    template<class TConnection,class TUser>
    __attribute__((noinline)) inline void TcpServer<TConnection,TUser>::onReceive(TcpSegmentEvent& event) {

      // we are only interested in segments sent to our port

      if(event.destinationPort!=_listeningPort)
        return;

      // we are only interested in segments that have the SYN flag set (but no ACK)

      if(!(event.tcpHeader.hasSyn() && !event.tcpHeader.hasAck()))
        return;

      // this event is handled here

      event.handled=true;

      // we are doing nothing unless we are started

      if(!_started)
        return;

      // if an existing connection already has this source/dest port combo then this
      // segment is a retransmit and we're going to drop it

      TcpFindConnectionNotificationEvent findconn(event.ipPacket.header->ip_sourceAddress,event.sourcePort,event.destinationPort);
      _networkUtilityObjects.NetworkNotificationEventSender.raiseEvent(findconn);

      // if there's a connection, return now as we have a dupe SYN

      if(findconn.tcpConnection!=nullptr)
        return;

      // if we've hit the maximum number of connections then we have to ignore it

      if(_connectionCount==_params.tcp_maxConnectionsPerServer)
        return;

      // new connection to record

      _connectionCount++;

      // create a new TcpConnection to handle the interaction
      // there is 2 phase construction here so that the user's derivation of TcpConnection does
      // not have to be aware of the objects required by the base class

      TConnection *connection=ConnectionCreator<TConnection,TUser>::createConnection(_connectionParams,_userptr);

      if(!connection->initialise(
            _networkUtilityObjects,
            _tcpEvents,
            event,
            _segmentSizeLimit,
            _additionalHeaderSize)) {

        delete connection;
        return;
      }

      // send a notification event so the caller can claim the connection

      TcpAcceptEvent acceptEvent(*this,connection);
      TcpAcceptEventSender.raiseEvent(acceptEvent);

      // if the caller does not want this connection then we delete it

      if(!acceptEvent.accepted)
        delete connection;
    }
  }
}
