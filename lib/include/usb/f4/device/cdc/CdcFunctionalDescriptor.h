/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB CDC functional descriptor base
     */

    struct CdcFunctionalDescriptor {

      /**
       * CDC functional descriptor type enumeration
       */

      enum {
        HEADER = 0x00,
        CALL_MANAGEMENT = 0x01,
        ABSTRACT_CONTROL_MANAGEMENT = 0x02,
        DIRECT_LINE_MANAGEMENT = 0x03,
        TELEPHONE_RINGER = 0x04,
        TELEPHONE_CALL_AND_LINE_STATE_REPORTING_CAPABILITIES = 0x05,
        UNION = 0x06,
        COUNTRY_SELECTION = 0x07,
        TELEPHONE_OPERATIONAL_MODES = 0x08,
        USB_TERMINAL = 0x09,
        NETWORK_CHANNEL_TERMINAL = 0x0a,
        PROTOCOL_UNIT = 0x0b,
        EXTENSION_UNIT = 0x0c,
        MULTI_CHANNEL_MANAGEMENT = 0x0d,
        CAPI_CONTROL_MANAGEMENT = 0x0e,
        ETHERNET_NETWORKING = 0x0f,
        ATM_NETWORKING = 0x10,
        WIRELESS_HANDSET_CONTROL_MODEL = 0x11
      };

      DescriptorHeader header;
      uint8_t bDescriptorSubtype;

      CdcFunctionalDescriptor(uint8_t subtype,uint8_t len)
        : header(len,0x24),
          bDescriptorSubtype(subtype) {       // CS_INTERFACE = 0x24
      }
    } __attribute__((packed));
  }
}
