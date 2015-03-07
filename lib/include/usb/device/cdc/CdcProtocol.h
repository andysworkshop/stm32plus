/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Protocol types for CDC devices
     */

    enum class CdcProtocol : uint8_t {
      NONE = 0x00,
      COMMON_AT_COMMANDS = 0x01,
      PCCA101_AT_COMMANDS = 0x02,
      PCCA101_ANNEX_O_AT_COMMANDS = 0x03,
      GSM_0707_AT_COMMANDS = 0x04,
      GPP_27007_AT_COMMANDS = 0x05,
      TIA_CDMA_AT_COMMANDS = 0x06,
      ETHERNET_EMULATION_MODE = 0x07,

      EXTERNAL = 0xFE,
      VENDOR = 0xFF
    };
  }
}
