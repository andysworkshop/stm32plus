/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Subclass codes for CDC devices
     */

    enum class CdcSubClass : uint8_t {
      RESERVED = 0,
      DIRECT_LINE_CONTROL_MODEL = 0x1,
      ABSTRACT_CONTROL_MODEL = 0x2,
      TELEPHONE_CONTROL_MODEL = 0x3,
      MULTI_CHANNEL_CONTROL_MODEL = 0x4,
      CAPI_CONTROL_MODEL = 0x5,
      ETHERNET_NETWORKING_CONTROL_MODEL = 0x6,
      ATM_NETWORKING_CONTROL_MODEL = 0x7,
      WIRELESS_HANDSET_CONTROL_MODEL = 0x8,
      DEVICE_MANAGEMENT = 0x9,
      MOBILE_DIRECT_LINE_MODEL = 0xa,
      OBEX = 0xb
    };
  }
}
