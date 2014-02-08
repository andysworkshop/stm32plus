/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a TCP connection being released
     */

    class TcpConnection;

    struct TcpConnectionReleasedEvent : NetEventDescriptor {

      const TcpConnection& connection;

      /**
       * Constructor
       * @param c The connection being released
       */

      TcpConnectionReleasedEvent(const TcpConnection& c)
        : NetEventDescriptor(NetEventDescriptor::NetEventType::TCP_CONNECTION_RELEASED),
          connection(c) {
      }
    };
  }
}

