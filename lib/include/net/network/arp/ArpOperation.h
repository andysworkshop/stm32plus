/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * ARP operation codes - always big-endian
     */

    enum class ArpOperation : uint16_t {
      REQUEST = 0x0100,       //!< REQUEST
      REPLY   = 0x0200,       //!< REPLY
    };
  }
}
