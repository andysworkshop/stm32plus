/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Class to manage a statically sized array of connections. Typically a TCP server
     * would use this to keep track of activity from its clients. The connection array
     * would be sized to be equal to the configured maximum number of connections that
     * the server has been configured to simultaneously accept.
     *
     * The memory required for an instance of this class is sizeof(TcpConnectionArray)
     * plus 4*connection-count
     */

    template<class TConnection>
    class TcpConnectionArray {

      public:

        /**
         * Possible error codes
         */

        enum {
          E_FULL = 1,             ///< no more slots available
          E_NOT_FOUND,            ///< connection not found
          E_ALREADY_SUBSCRIBED    ///< cannot subscribed to accept events twice
        };

      protected:
        NetworkUtilityObjects& _networkUtilityObjects;
        TConnection **_connections;
        uint16_t _connectionCount;
        uint16_t _lastFree;
        uint16_t _index;
        TcpServerBase *_subscribedServer;
        uint32_t _idleTimeout;

      protected:
        void initialise();
        bool handleFail(TConnection *conn,TcpWaitState state,TConnection **outputConnection,TcpWaitState *outputState) const;

        void onNotification(NetEventDescriptor& ned);
        void onAccept(TcpAcceptEvent& event);
        void unsubscribeServer();

      public:
        TcpConnectionArray(TcpServerBase& server);
        TcpConnectionArray(NetworkUtilityObjects& netUtils,uint16_t count);
        ~TcpConnectionArray();

        bool autoAdd(TcpServerBase& server);
        bool add(TConnection& conn);
        bool remove(const TcpConnection& conn);

        bool wait(TcpWaitState states,uint32_t timeout,TConnection** outputConnection=nullptr,TcpWaitState *outputState=nullptr);
    };


    /**
     * Construct against an existing TCP server. This constructor will automatically subscribe to
     * connections created by this server and will size the internal connections array to match
     * the max connection count of the server
     */

    template<class TConnection>
    inline TcpConnectionArray<TConnection>::TcpConnectionArray(TcpServerBase& server)
      : _networkUtilityObjects(server.getNetworkUtilityObjects()) {

      _connectionCount=server.getParameters().tcp_maxConnectionsPerServer;
      initialise();

      autoAdd(server);
    }


    /**
     * Constructor
     * @param count The maximum number of connections to manage
     */

    template<class TConnection>
    inline TcpConnectionArray<TConnection>::TcpConnectionArray(NetworkUtilityObjects& netUtils,uint16_t count)
      : _networkUtilityObjects(netUtils) {

      // set up the connection array

      _connectionCount=count;
      initialise();
    }


    /**
     * Initialise the array
     */

    template<class TConnection>
    inline void TcpConnectionArray<TConnection>::initialise() {

      _connections=reinterpret_cast<TConnection **>(malloc(sizeof(TConnection *)*_connectionCount));

      memset(_connections,0,sizeof(TConnection *)*_connectionCount);
      _lastFree=0;
      _index=0;
      _idleTimeout=0;

      // we're not subscribed to a server yet

      _subscribedServer=nullptr;

      // subscribe to network notification events so we know when a TCP server or connection is released

      _networkUtilityObjects.NetworkNotificationEventSender.insertSubscriber(
          NetworkNotificationEventSourceSlot::bind(this,&TcpConnectionArray<TConnection>::onNotification));
    }


    /**
     * Destructor, release array memory. We don't own the connection pointers
     * so leave those alone
     */

    template<class TConnection>
    inline TcpConnectionArray<TConnection>::~TcpConnectionArray() {

      // if we are subscribed to a server then unsubscribe

      unsubscribeServer();

      // unsubscribe from notification events

      _networkUtilityObjects.NetworkNotificationEventSender.removeSubscriber(
          NetworkNotificationEventSourceSlot::bind(this,&TcpConnectionArray<TConnection>::onNotification));

      // release the connections

      free(_connections);
    }


    /**
     * Unsubscribe from a server's accept events
     */

    template<class TConnection>
    inline void TcpConnectionArray<TConnection>::unsubscribeServer() {

      if(_subscribedServer)
        _subscribedServer->TcpAcceptEventSender.removeSubscriber(
            TcpAcceptEventSourceSlot::bind(this,&TcpConnectionArray<TConnection>::onAccept));

      _subscribedServer=nullptr;
      _idleTimeout=0;
    }


    /**
     * Auto-add all connections created by a given TCP server. This subscribes this
     * class to accept events from the TCP server. Only one TCP server can be auto-added.
     *
     * We subscribe to TCP server released events and will automatically remove our
     * subscription if the TCP server goes away
     * @param server The server to subscribe to
     * @return true if successfully subscribed
     */

    template<class TConnection>
    inline bool TcpConnectionArray<TConnection>::autoAdd(TcpServerBase& server) {

      // cannot be already subscribed

      if(_subscribedServer!=nullptr)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION_ARRAY,E_ALREADY_SUBSCRIBED);

      _subscribedServer=&server;
      _idleTimeout=server.getParameters().tcp_idleConnectionTimeout;

      // subscribe to accept events from the server

      server.TcpAcceptEventSender.insertSubscriber(
          TcpAcceptEventSourceSlot::bind(this,&TcpConnectionArray<TConnection>::onAccept));

      return true;
    }


    /**
     * A network notification arrived. We listen for 'release' notifications from connections and servers
     * so we can automatically clean up.
     * @param ned The notification descriptor
     */

    template<class TConnection>
    __attribute__((noinline)) inline void TcpConnectionArray<TConnection>::onNotification(NetEventDescriptor& ned) {

      // only interested in server released events when we are subscribed

      if(ned.eventType==NetEventDescriptor::NetEventType::TCP_SERVER_RELEASED) {

        if(_subscribedServer!=nullptr) {

          // unsubscribe from events raised by this server

          TcpServerReleasedEvent& tsre(static_cast<TcpServerReleasedEvent&>(ned));

          if(&tsre.server==_subscribedServer)
            unsubscribeServer();
        }
      }
      else if(ned.eventType==NetEventDescriptor::NetEventType::TCP_CONNECTION_RELEASED) {

        // remove this connection from the array

        TcpConnectionReleasedEvent& tcre(static_cast<TcpConnectionReleasedEvent&>(ned));
        remove(tcre.connection);
      }
    }


    /**
     * A connection-accept event has been received, auto-add it to the array
     * @param event The accept event
     */

    template<class TConnection>
    __attribute__((noinline)) inline void TcpConnectionArray<TConnection>::onAccept(TcpAcceptEvent& event) {

      // the accept must be from the server that we are subscribed to

      if(&event.server==_subscribedServer) {

        // auto-add the connection and mark it as being accepted

        if(add(static_cast<TConnection&>(*event.connection)))
          event.accepted=true;
      }
    }


    /**
     * Add a connection to the array
     */

    template<class TConnection>
    inline bool TcpConnectionArray<TConnection>::add(TConnection& conn) {

      uint16_t i,start;

      // need to rely on the internal state being immutable

      IrqSuspend suspender;

      i=start=_lastFree;

      for(;;) {

        if(_connections[i]) {

          // the connection slot is in use, try the next one or fail if we've done a complete wrap

          if(++i==_connectionCount)
            i=0;

          if(i==start)
            return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION_ARRAY,E_FULL);
        }
        else {

          // the connection is free, use it and update the next search start

          _connections[i]=&conn;

          if(++_lastFree==_connectionCount)
            _lastFree=0;

          return true;
        }
      }
    }


    /**
     * Remove a connection from the array. Often not needed because connections are automatically
     * removed when they are closed.
     * @param conn The connection to remove
     * @return true if it was found and removed
     */

    template<class TConnection>
    inline bool TcpConnectionArray<TConnection>::remove(const TcpConnection& conn) {

      uint16_t i;

      // need to rely on the internal state being immutable

      IrqSuspend suspender;

      for(i=0;i<_connectionCount;i++) {

        if(_connections[i]==&conn) {

          // found it. remove it and make a note of a guaranteed empty slot

          _connections[i]=nullptr;
          _lastFree=i;

          return true;
        }
      }

      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION_ARRAY,E_NOT_FOUND);
    }


    /**
     * wait() will go around the connections in round-robin fashion looking for connections
     * that are ready for (read/write) or closed, the desired states are passed in via the states parameter.
     * If a connection matches the required state then its handleRead() handleWrite() or handleClosed() method
     * is called and we move around the rest of the connections in the array in round-robin fashion until
     * the timeout expires. A zero value for the timeout means that it never expires.
     *
     * If a handleXXXX method returns false then this function returns false immediately and the connection
     * in question is returned in the outputConnection parameter and the state that returned false is returned
     * in the outputState parameter.
     *
     * @param states The states to test for
     * @param timeout Maximum time to go on testing states
     * @param[out] outputConnection the connection that returned false. You can pass nullptr if you don't care for this
     * @paran[out] outputState the connection state when it returned false. You can pass nullptr if you don't care for this
     */

    template<class TConnection>
    inline bool TcpConnectionArray<TConnection>::wait(TcpWaitState states,
                                                      uint32_t timeout,
                                                      TConnection** outputConnection,
                                                      TcpWaitState *outputState) {


      uint32_t now;
      TConnection *conn;

      // get the current time

      if(timeout)
        now=MillisecondTimer::millis();
      else
        now=0;

      // loop while something happens

      for(;;) {

        // have we timed out?

        if(timeout && MillisecondTimer::hasTimedOut(now,timeout))
          return true;

        // increment the search pointer

        if(_index==_connectionCount-1)
          _index=0;
        else
          _index++;

        // read is possible when there is some data in the buffer

        if((conn=_connections[_index])!=nullptr && (states & TcpWaitState::READ)!=TcpWaitState::NONE && conn->getDataAvailable()>0) {
          if(!conn->handleRead())
            return handleFail(conn,TcpWaitState::READ,outputConnection,outputState);
        }

        // write is possible when the connection is ESTABLISHED (zero bytes may be written in a zero-window state)

        if((conn=_connections[_index])!=nullptr && (states & TcpWaitState::WRITE)!=TcpWaitState::NONE && conn->getConnectionState().state==TcpState::ESTABLISHED) {
          if(!conn->handleWrite())
            return handleFail(conn,TcpWaitState::WRITE,outputConnection,outputState);
        }

        // callback is possible at any time

        if((conn=_connections[_index])!=nullptr && (states & TcpWaitState::CALLBACK)!=TcpWaitState::NONE) {
          if(!conn->handleCallback())
            return handleFail(conn,TcpWaitState::CALLBACK,outputConnection,outputState);
        }

        // closed is called when local or remote end is closed

        if((conn=_connections[_index])!=nullptr && (states & TcpWaitState::CLOSED)!=TcpWaitState::NONE && (conn->isRemoteEndClosed() || conn->isLocalEndClosed())) {
          if(!conn->handleClosed())
            return handleFail(conn,TcpWaitState::WRITE,outputConnection,outputState);
        }

        // has the connection timed out?

        if(_idleTimeout && (conn=_connections[_index])!=nullptr && MillisecondTimer::hasTimedOut(conn->getLastActiveTime(),_idleTimeout)) {

          // we'll get a callback via our connection-released notification subscription
          // and we'll remove it from the array automatically

          delete conn;
        }
      }
    }


    /**
     * Set parameters for a fail return
     * @param conn The connection that was called
     * @param state The state that it was in
     * @param outputConnection Where to store conn
     * @param outputState Where to store state
     * @return Always false
     */

    template<class TConnection>
    inline bool TcpConnectionArray<TConnection>::handleFail(TConnection *conn,
                                                            TcpWaitState state,
                                                            TConnection **outputConnection,
                                                            TcpWaitState *outputState) const {
      if(outputConnection)
        *outputConnection=conn;

      if(outputState)
        *outputState=state;

      return false;
    }
  }
}
