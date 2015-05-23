/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event that is sent when a TCP connection state has changed. For example when a client
     * asynchronous connection attempt moves away from SYN_SENT to ESTABLISHED or CLOSED.
     */

    struct TcpConnectionStateChangedEvent : NetEventDescriptor {

      IpAddress destAddress;
      uint16_t destPort;
      TcpState oldState;
      TcpState newState;

      TcpConnectionStateChangedEvent(const IpAddress& address,uint16_t port,TcpState old_state,TcpState new_state)
        : NetEventDescriptor(NetEventType::TCP_CONNECTION_STATE_CHANGED),
          destAddress(address),
          destPort(port),
          oldState(old_state),
          newState(new_state) {
      }
    };
  }
}
