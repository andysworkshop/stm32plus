/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * USB language descriptor. The standard supports multiple languages but the ST SDK is
   * limited to one.
   */

  struct UsbLanguageDescriptor {

    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wLanguageId;

    /**
     * Constructor
     */

    UsbLanguageDescriptor() {

      static_assert(sizeof(UsbLanguageDescriptor)==USB_LEN_LANGID_STR_DESC,"Compilation error: sizeof(UsbLanguageDescriptor)!=4");

      bLength=USB_LEN_LANGID_STR_DESC;
      bDescriptorType=USB_DESC_TYPE_STRING;
    }
  } __attribute((packed));
}
