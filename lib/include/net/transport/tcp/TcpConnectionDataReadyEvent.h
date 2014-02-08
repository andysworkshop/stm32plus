/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * TCP connection data ready event. This event signifies that we have buffered
     * some data from the remote end that the application should consume.
     */

    class TcpConnection;

    struct TcpConnectionDataReadyEvent : NetEventDescriptor {

      /**
       * Reference to the TCP connection object.
       */

      TcpConnection& connection;

      /**
       * Constructor
       * @param The connection reference
       */

      TcpConnectionDataReadyEvent(TcpConnection& c)
        : NetEventDescriptor(NetEventDescriptor::NetEventType::TCP_CONNECTION_DATA_READY),
          connection(c) {
      }
    };
  }
}
