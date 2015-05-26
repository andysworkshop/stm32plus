/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Implementation of the UDP protocol over IP. Datagrams are received asynchronously from the IP
     * layer and passed on to the upper layers. Functionality is provided for sending and receiving
     * datagrams synchronously to the caller.
     */

    template<class TNetworkLayer>
    class Tcp : public virtual TNetworkLayer,
                public TcpEvents {

      public:
        /**
         * Error codes
         */

        enum {
          E_TOO_MANY_SERVERS = 1,       ///< tcp_maxServers limit hit
          E_PORT_IN_USE,                ///< trying to bind to a port that's already claimed
          E_TIMEOUT,                    ///< timed out while waiting to connect
          E_CONNECT_FAILED              ///< connection failed
        };


        /**
         * Parameters class
         */

        struct Parameters : TcpServerBase::Parameters {

          uint16_t tcp_maxServers;                ///< maximum number of servers at any one time. default is 5
          uint16_t tcp_msl;                       ///< maximum segment lifetime, in seconds. default is 30
          uint16_t tcp_connectRetryInterval;      ///< the time, in millis to wait for a SYN-ACK before sending another. Default is 4000.
          uint16_t tcp_connectMaxRetries;         ///< number of times to retry a connect if SYN-ACK not received. Default is 5.

          /**
           * Constructor
           */

          Parameters() {
            tcp_maxServers=5;
            tcp_msl=30;
            tcp_connectRetryInterval=4000;
            tcp_connectMaxRetries=5;
          }
        };

      protected:
        Parameters _params;
        uint16_t _serverCount;
        std::slist<TcpClosingConnectionState> _closingConnections;

      protected:
        void onNotification(NetEventDescriptor& ned);
        void onReceive(IpPacketEvent& event);
        void onTick(NetworkIntervalTickData& nitd);
        void handleConnectionReleased(const TcpConnectionReleasedEvent& tcre);
        bool rejectWithRst(const TcpSegmentEvent& event);
        void handleFinWait1(const TcpHeader& header,TcpConnectionState& rstate);
        void handleFinWait2(const TcpHeader& header,TcpConnectionState& rstate);
        bool handleLastAck(const TcpHeader& header,TcpConnectionState& rstate);

      public:
        template<class TConnection,class TUser=void>
        bool tcpCreateServer(uint16_t port,TcpServer<TConnection,TUser> *&server,TUser *userptr=nullptr);

        template<class TConnection>
        bool tcpConnect(const IpAddress& remoteAddress,
                        uint16_t remotePort,
                        TConnection *& connection);

        template<class TConnection>
        bool tcpConnect(const IpAddress& remoteAddress,
                        uint16_t localPort,
                        uint16_t remotePort,
                        TConnection *& connection);

        template<class TConnection>
        bool tcpConnectAsync(const IpAddress& remoteAddress,
                        uint16_t remotePort,
                        TConnection *& connection);

        template<class TConnection>
        bool tcpConnectAsync(const IpAddress& remoteAddress,
                        uint16_t localPort,
                        uint16_t remotePort,
                        TConnection *& connection);

        bool initialise(const Parameters& params);
        bool startup();

        Parameters& tcpGetParameters();
    };


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Tcp<TNetworkLayer>::initialise(const Parameters& params) {

      uint16_t tickSeconds;

      // save parameters

      _params=params;
      _serverCount=0;

      // subscribe to notify events from the network

      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Tcp<TNetworkLayer>::onNotification));

      // subscribe to packet events from the IP module

      this->IpReceiveEventSender.insertSubscriber(IpReceiveEventSourceSlot::bind(this,&Tcp<TNetworkLayer>::onReceive));

      // subscribe to the second ticker so we can clean up closed connections. the ticker has a granularity of
      // 10 seconds or msl/4, whichever is the greater. the lower limit prevents us taking up too much CPU checking
      // whether closed connections should be cleaned up.

      tickSeconds=std::max(10,params.tcp_msl/4);
      this->subscribeIntervalTicks(tickSeconds,NetworkIntervalTicker::TickIntervalSlotType::bind(this,&Tcp<TNetworkLayer>::onTick));

      return true;
    }


    /**
     * Startup the class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Tcp<TNetworkLayer>::startup() {
      return true;
    }


    /**
     * Network receive event
     * @param ned The event descriptor
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Tcp<TNetworkLayer>::onReceive(IpPacketEvent& ipe) {

      // must be a TCP packet

      if(ipe.ipPacket.header->ip_hdr_protocol!=IpProtocol::TCP)
        return;

      // sending an event of our own saves each interested recipient from having
      // to decode the ip structure and reduces the number of events sent to something
      // that only cares for TCP receive events

      TcpHeader *header=reinterpret_cast<TcpHeader *>(ipe.ipPacket.payload);
      uint8_t *data=ipe.ipPacket.payload+header->getDataOffset();
      uint16_t datalen=ipe.ipPacket.payloadLength-header->getHeaderSize();

      // send the event

      TcpSegmentEvent event(ipe.ipPacket,
                             *header,
                             data,
                             datalen,
                             NetUtil::ntohs(header->tcp_sourcePort),
                             NetUtil::ntohs(header->tcp_destinationPort));

      TcpReceiveEventSender.raiseEvent(event);

      // if a connection or server handled it then we don't need to go further

      if(event.handled)
        return;

      // check if this segment is for one of the closing connections

      std::slist<TcpClosingConnectionState>::iterator it,previt;
      bool notfound;

      {
        IrqSuspend suspender;

        // find the connection, under pre-emption prevention

        for(it=previt=_closingConnections.begin();it!=_closingConnections.end();it++) {

          if(it->matches(event))
            break;

          previt=it;
        }

        notfound=it==_closingConnections.end();
      }


      // if not one of ours then a segment has arrived for an unknown connection
      // we reply with RST

      if(notfound)
        rejectWithRst(event);
      else {
        // do something with it

        switch(it->state) {

          case TcpState::FIN_WAIT_1:
            handleFinWait1(*header,*it);
            break;

          case TcpState::FIN_WAIT_2:
            handleFinWait2(*header,*it);
            break;

          case TcpState::LAST_ACK:

            if(handleLastAck(*header,*it)) {

              IrqSuspend suspender;

              if(it==_closingConnections.begin())
                _closingConnections.erase(_closingConnections.begin());
              else
                _closingConnections.erase_after(previt);
            }
            break;

          default:
            break;
        }
      }
    }


    /**
     * Reject this segment with an RST. This is IRQ code
     * @param event The segment event
     * @return true if it was sent
     */

    template<class TNetworkLayer>
    inline bool Tcp<TNetworkLayer>::rejectWithRst(const TcpSegmentEvent& event) {

      uint32_t sequence,ack;

      // don't RST a segment that was itself a RST

      if(event.tcpHeader.hasRst())
        return false;

      // calculate sequence and ack for this RST (see RFC793 for reasoning)

      if(event.tcpHeader.hasAck())
        sequence=NetUtil::ntohl(event.tcpHeader.tcp_ackNumber);
      else
        sequence=0;

      ack=NetUtil::ntohl(event.tcpHeader.tcp_sequenceNumber)+event.payloadLength;

      // create a NetBuffer to hold the RST segment

      NetBuffer *nb=new NetBuffer(
          this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize()+TcpHeader::getNoOptionsHeaderSize(),
          0);

      // construct the header

      TcpHeader *header=reinterpret_cast<TcpHeader *>(nb->moveWritePointerBack(TcpHeader::getNoOptionsHeaderSize()));

      header->initialise(event.destinationPort,         // was sent to this local port
                         event.sourcePort,              // came from this remote port
                         sequence,
                         ack,
                         0,                             // zero window
                         TcpHeaderFlags::RST);

      // ask the IP layer to send the packet

      IpTransmitRequestEvent iptre(
            nb,
            event.ipPacket.header->ip_sourceAddress,
            IpProtocol::TCP);

      this->NetworkSendEventSender.raiseEvent(iptre);
      return iptre.succeeded;
    }


    /**
     * Notification from our subscription to notification events
     * @param ned The event descriptor
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Tcp<TNetworkLayer>::onNotification(NetEventDescriptor& ned) {

      // handle a TCP server being released by releasing its listening port

      if(ned.eventType==NetEventDescriptor::NetEventType::TCP_SERVER_RELEASED) {

        uint16_t port;

        // get the port from the message

        port=static_cast<TcpServerReleasedEvent&>(ned).server.getListeningPort();

        // most likely it's on a defined port but could be on an ephemeral (e.g. FTP data connection)

        if(this->ip_releaseDefinedPort(port) || this->ip_releaseEphemeralPort(port))
          _serverCount--;
      }
      else if(ned.eventType==NetEventDescriptor::NetEventType::TCP_CONNECTION_RELEASED)
        handleConnectionReleased(static_cast<TcpConnectionReleasedEvent&>(ned));
    }


    /**
     * Network interval ticker callback. This is IRQ code.
     * @param nitd The tick data
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Tcp<TNetworkLayer>::onTick(NetworkIntervalTickData& nitd) {

      std::slist<TcpClosingConnectionState>::iterator previt,it;

      // iterate the closing connections

      for(it=previt=_closingConnections.begin();it!=_closingConnections.end();) {

        // check if it's expired

        if(nitd.timeNow>it->cleanupTime) {

          // if the state is not CLOSED or TIME_WAIT then the close sequence has not
          // completed as it should. we send a RST to the other end to tell it to shutdown.

          if(it->state!=TcpState::TIME_WAIT && it->state!=TcpState::CLOSED)
            it->sendRstAck(*this,0);

          // if the local port is ephemeral then release it

          if(it->localPortIsEphemeral)
            this->ip_releaseEphemeralPort(it->localPort);

          // if this is the head of the list, remove it and continue again from the start

          if(it==_closingConnections.begin()) {
            _closingConnections.erase(_closingConnections.begin());
            it=previt=_closingConnections.begin();
          }
          else {

            // erase the current one efficiently. erase_after returns iterator AFTER the one removed
            // previt does not change

            it=_closingConnections.erase_after(previt);
          }
        }
        else {

          // update both iterators

          previt=it;
          it++;
        }
      }
    }


    /**
     * A connection is released. The destructor for the connection is underway and we must now
     * decide if we need to go into the closing sequence based on the state of this connection.
     * @param tcre The connection released event
     */

    template<class TNetworkLayer>
    inline void Tcp<TNetworkLayer>::handleConnectionReleased(const TcpConnectionReleasedEvent& tcre) {

      switch(tcre.connection.getConnectionState().state) {

        case TcpState::SYN_RCVD:
        case TcpState::ESTABLISHED:
        case TcpState::CLOSE_WAIT:
          break;

        default:

          // if the local port is ephemeral then release it

          if(tcre.connection.getConnectionState().localPortIsEphemeral)
            this->ip_releaseEphemeralPort(tcre.connection.getConnectionState().localPort);

          return;
      }

      // add to the list (struct copy)

      std::slist<TcpClosingConnectionState>::iterator it;

      {
        IrqSuspend suspender;

        _closingConnections.push_front(
          TcpClosingConnectionState(tcre.connection.getConnectionState(),
                                     this->getRtc().getTick()+(_params.tcp_msl*2)));

        it=_closingConnections.begin();
      }

      // if we are the active closer or we've received a FIN from the other end
      // then we need to send a FIN

      switch(it->state) {

        case TcpState::SYN_RCVD:          // we are the active closer in these cases
        case TcpState::ESTABLISHED:
          it->changeState(*this,TcpState::FIN_WAIT_1);
          it->sendFinAck(*this,0);
          it->txWindow.sendNext++;
          break;

        case TcpState::CLOSE_WAIT:        // we are the passive closer in these cases
          it->changeState(*this,TcpState::LAST_ACK);
          it->sendFinAck(*this,0);
          it->txWindow.sendNext++;
          break;

        default:
          return;
      }
    }


    /**
     * Handle the segment received while in FIN_WAIT_1. This is IRQ code.
     * We are expecting to receive an ACK for our FIN
     * @param header the TCP header
     * @param rstate the state variables
     */

    template<class TNetworkLayer>
    inline void Tcp<TNetworkLayer>::handleFinWait1(const TcpHeader& header,TcpConnectionState& rstate) {

      // must be an ACK and the sequence number must match

      if(!header.hasAck() || NetUtil::ntohl(header.tcp_ackNumber)!=rstate.txWindow.sendNext)
        return;

      rstate.txWindow.sendUnacknowledged=rstate.txWindow.sendNext;

      // new state is FIN_WAIT_2

      rstate.changeState(*this,TcpState::FIN_WAIT_2);
    }


    /**
     * Handle the segment received while in FIN_WAIT_2. This is IRQ code.
     * We are expecting to receive a FIN from the other side.
     * @param header the TCP header
     * @param rstate the remote state variables
     */

    template<class TNetworkLayer>
    inline void Tcp<TNetworkLayer>::handleFinWait2(const TcpHeader& header,TcpConnectionState& rstate) {

      // must be an ACK and the sequence number must match

      if(!header.hasFin())
        return;

      // update the remote sequence number and send the ACK for that

      rstate.rxWindow.receiveNext=NetUtil::ntohl(header.tcp_sequenceNumber)+1;
      rstate.sendAck(*this,0);

      // new state is TIME_WAIT

      rstate.changeState(*this,TcpState::TIME_WAIT);
    }


    /**
     * Handle the segment received while in LAST_ACK. This is IRQ code.
     * We are expecting to receive an ACK from the other side.
     * @param header the TCP header
     * @param rstate the remote state variables
     * @return true if the connection is closed and can be removed
     */

    template<class TNetworkLayer>
    inline bool Tcp<TNetworkLayer>::handleLastAck(const TcpHeader& header,TcpConnectionState& rstate) {

      // must be an ACK and the sequence number must match

      if(!header.hasAck() || NetUtil::ntohl(header.tcp_ackNumber)!=rstate.txWindow.sendNext)
        return false;

      // we're done

      rstate.changeState(*this,TcpState::CLOSED);
      return true;
    }


    /**
     * Create a new TCP server that will listen on the specified port. The port number must not be in
     * the ephemeral range (default 49152 to 65535). The ephemeral range is customisable through the
     * module parameters.
     * @param port The port number to listen on
     * @param[out] server The server pointer
     * @param userptr Optional pointer that you would like to be passed to your TConnection's constructor. This allows you to pass state around.
     * @tparam TConnection Your most-derived subclass of TcpConnection. The server will create instances of this.
     * @tparam TUser optional type of the parameter to your TConnection's constructor, if your TConnection takes a constructor.
     * @return true if it worked, and is now listening.
     */

    template<class TNetworkLayer>
    template<class TConnection,class TUser>
    inline bool Tcp<TNetworkLayer>::tcpCreateServer(uint16_t port,
                                                    TcpServer<TConnection,TUser> *&server,
                                                    TUser *userptr) {

      // check that the server limit has not been hit

      if(_serverCount==_params.tcp_maxServers)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP,E_TOO_MANY_SERVERS);

      // try to claim the port

      if(!this->ip_acquireDefinedPort(port))
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP,E_PORT_IN_USE);

      // create the server

      server=new TcpServer<TConnection,TUser>(
          port,
          *this,
          *this,
          _params,
          this->getDatalinkMtuSize()-IpPacketHeader::getNoOptionsHeaderSize()-TcpHeader::getNoOptionsHeaderSize(),
          this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize(),
          userptr);

      _serverCount++;
      return true;
    }


    /**
     * Connect to a remote endpoint and port. The interval between retries and the number of retries that are
     * attempted can be configured in the Parameters class. A random local port from the ephemeral range will
     * be selected.
     * This is not IRQ safe.
     * @param remoteAddress The remote IP address to connect to
     * @param remotePort The remote server port to connect to
     * @param connection The new connection object. Delete when you're done.
     * @tparam TConnection Your most-derived subclass of TcpConnection that you're using to handle the connection.
     * @return true if it works, false if not
     */

    template<class TNetworkLayer>
    template<class TConnection>
    bool Tcp<TNetworkLayer>::tcpConnect(const IpAddress& remoteAddress,
                                        uint16_t remotePort,
                                        TConnection *&connection) {

      uint16_t localPort;

      // get a port from the ephemeral range

      if(!this->ip_acquireEphemeralPort(localPort))
        return false;

      // connect with all parameters

      if(!tcpConnect(remoteAddress,localPort,remotePort,connection)) {

        // didn't work, release the local port

        this->ip_releaseEphemeralPort(localPort);
        return false;
      }

      // worked

      return true;
    }


    /**
     * Connect to a remote endpoint and port. The interval between retries and the number of retries that are
     * attempted can be configured in the Parameters class. You specify the local port number, which must be from
     * the ephemeral range. This is not IRQ safe.
     * @param remoteAddress The remote IP address to connect to
     * @param remotePort The remote server port to connect to
     * @param connection The new connection object. Delete when you're done.
     * @tparam TConnection Your most-derived subclass of TcpConnection that you're using to handle the connection.
     * @return true if it works, false if not
     */

    template<class TNetworkLayer>
    template<class TConnection>
    bool Tcp<TNetworkLayer>::tcpConnect(const IpAddress& remoteAddress,
                                        uint16_t localPort,
                                        uint16_t remotePort,
                                        TConnection *&connection) {
      uint16_t retry;

      // create and initialise the new connection. this will send the first SYN

      connection=nullptr;
      scoped_ptr<TConnection> conn(new TConnection);

      if(!conn->initialise(
                        *this,
                        *this,
                        remoteAddress,
                        localPort,
                        remotePort,
                        this->getDatalinkMtuSize()-IpPacketHeader::getNoOptionsHeaderSize()-TcpHeader::getNoOptionsHeaderSize(),
                        this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize()))
        return false;

      // _params.tcp_connectMaxRetries really does mean retries. That is, there will be the initial
      // attempt followed by that many retries.

      retry=0;

      do {

        // now wait for the state to move away from SYN_SENT

        if(conn->waitForStateChange(TcpState::SYN_SENT,_params.tcp_connectRetryInterval)) {

          // anything but ESTABLISHED means failure. most likely the remote end RST'd our SYN

          if(conn->getConnectionState().state==TcpState::ESTABLISHED) {
            connection=conn.release();
            return true;
          }

          // failed

          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP,E_CONNECT_FAILED);
        }

        // send the next SYN

        if(retry!=_params.tcp_connectMaxRetries)
          conn->sendSyn();

      } while(retry++<_params.tcp_connectMaxRetries);

      // retries exhausted

      return this->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP,E_TIMEOUT);
    }


    /**
     * Connect to a remote endpoint and port. The interval between retries and the number of retries that are
     * attempted can be configured in the Parameters class. A random local port from the ephemeral range will
     * be selected. This asynchronous method requires you to subscribe to the TcpConnectionStateChangedEvent
     * This is not IRQ safe.
     * @param remoteAddress The remote IP address to connect to
     * @param remotePort The remote server port to connect to
     * @param connection The new connection object. Delete when you're done.
     * @tparam TConnection Your most-derived subclass of TcpConnection that you're using to handle the connection.
     * @return true if it works, false if not
     */

    template<class TNetworkLayer>
    template<class TConnection>
    bool Tcp<TNetworkLayer>::tcpConnectAsync(const IpAddress& remoteAddress,
                                        uint16_t remotePort,
                                        TConnection *&connection) {

      uint16_t localPort;

      // get a port from the ephemeral range

      if(!this->ip_acquireEphemeralPort(localPort))
        return false;

      // connect with all parameters

      if(!tcpConnectAsync(remoteAddress,localPort,remotePort,connection)) {

        // didn't work, release the local port

        this->ip_releaseEphemeralPort(localPort);
        return false;
      }

      // worked

      return true;
    }


    /**
     * Connect to a remote endpoint and port. The interval between retries and the number of retries that are
     * attempted can be configured in the Parameters class. You specify the local port number, which must be from
     * the ephemeral range. This asynchronous method requires you to subscribe to the TcpConnectionStateChangedEvent
     * and look out for a move away from SYN_SENT to ESTABLISHED in the successful case or anything else in
     * the case of an error. You must handle timeout and retry logic yourself by calling sendSyn() when you want
     * to retry.
     * This is not IRQ safe.
     * @param remoteAddress The remote IP address to connect to
     * @param remotePort The remote server port to connect to
     * @param connection The new connection object. Delete when you're done.
     * @tparam TConnection Your most-derived subclass of TcpConnection that you're using to handle the connection.
     * @return true if it works, false if not
     */

    template<class TNetworkLayer>
    template<class TConnection>
    bool Tcp<TNetworkLayer>::tcpConnectAsync(const IpAddress& remoteAddress,
                                        uint16_t localPort,
                                        uint16_t remotePort,
                                        TConnection *&connection) {

      // create and initialise the new connection. this will send the first SYN

      connection=nullptr;
      connection=new TConnection;

      if(!connection->initialise(
                        *this,
                        *this,
                        remoteAddress,
                        localPort,
                        remotePort,
                        this->getDatalinkMtuSize()-IpPacketHeader::getNoOptionsHeaderSize()-TcpHeader::getNoOptionsHeaderSize(),
                        this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize())) {

        // failed, clean up

        delete connection;
        connection=nullptr;
        return false;
      }

      return true;
    }


    /**
     * Get a reference to the TCP parameters object
     * @return A reference to the parameters
     */

    template<class TNetworkLayer>
    typename Tcp<TNetworkLayer>::Parameters& Tcp<TNetworkLayer>::tcpGetParameters() {
      return _params;
    }
  }
}

