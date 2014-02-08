/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a TCP server being released
     */

    class TcpServerBase;

    struct TcpServerReleasedEvent : NetEventDescriptor {

      const TcpServerBase& server;

      /**
       * Constructor
       * @param s The server being released
       */

      TcpServerReleasedEvent(const TcpServerBase& s)
        : NetEventDescriptor(NetEventDescriptor::NetEventType::TCP_SERVER_RELEASED),
          server(s) {
      }
    };
  }
}

