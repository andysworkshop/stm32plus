/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * State of a TCP connection as illustrated in the state diagram from
     * TCP/IP Illustrated by W. Richard Stevens
     */

    enum class TcpState : uint8_t {
      NONE,
      CLOSED,
      SYN_RCVD,
      SYN_SENT,
      ESTABLISHED,
      CLOSE_WAIT,
      CLOSING,
      FIN_WAIT_1,
      FIN_WAIT_2,
      TIME_WAIT,
      LAST_ACK
    };
  }
}
