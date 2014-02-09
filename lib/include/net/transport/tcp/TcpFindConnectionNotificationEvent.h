/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event used to find an existing TCP connection that matches
     * the given source and destination ports
     */

    class TcpConnection;

    struct TcpFindConnectionNotificationEvent : NetEventDescriptor {

      const IpAddress& remoteAddress;       ///< the remote address
      uint16_t localPort;                     ///< local port
      uint16_t remotePort;                    ///< remote port

      TcpConnection *tcpConnection;           ///< the connection if found, nullptr if not


      /**
       * Constructor
       * @param raddr The remote address
       * @param sPort The source port (remote end)
       * @param dPort The destination port (local end)
       */

      TcpFindConnectionNotificationEvent(const IpAddress& raddr,uint16_t sPort,uint16_t dPort)
        : NetEventDescriptor(NetEventType::TCP_FIND_CONNECTION),
          remoteAddress(raddr),
          localPort(dPort),
          remotePort(sPort),
          tcpConnection(nullptr) {
      }
    };
  }
}

