/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * TCP connection closed event. This event signifies that the remote end has closed and will
     * not be sending any more data. It is legal to send data to the remote end in this state but
     * most likely you'll want to delete your TcpConnection object, thereby closing your end and
     * cleaning up.
     */

    class TcpConnection;

    struct TcpConnectionClosedEvent : NetEventDescriptor {

      /**
       * Reference to the TCP connection object.
       */

      TcpConnection& connection;


      /**
       * Constructor
       * @param The connection reference
       */

      TcpConnectionClosedEvent(TcpConnection& c)
        : NetEventDescriptor(NetEventDescriptor::NetEventType::TCP_CONNECTION_CLOSED),
          connection(c) {
      }
    };
  }
}
