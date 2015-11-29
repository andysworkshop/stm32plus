/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * MSC protocol codes
     */

    enum class MscProtocol : uint8_t {
      CBI_WITH_CCI = 0,
      CBI_WITHOUT_CCI = 1,
      BOT = 0x50,
      UAS = 0x62,
      VENDOR = 0xff
    };
  }
}
