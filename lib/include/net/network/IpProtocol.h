/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * IP protocol numbers. Not all of them.
     */

    enum class IpProtocol : uint8_t {
      ICMP = 0x01,
      TCP   = 0x06,
      UDP   = 0x11
    };
  }
}

