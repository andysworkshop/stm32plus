/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Known EtherType values used in this implementation
     */

    enum class EtherType : uint16_t {

      IP = 0x0800,          ///< Internet protocol v4
      ARP = 0x0806          ///< Address resolution protocol
    };
  }
}
