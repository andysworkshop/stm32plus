/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/usb/device/hid.h"


namespace stm32plus {
  namespace usb {

    /**
     * The mouse report descriptor bytes
     */

    const uint8_t MouseReportDescriptor[50]={
      0x05, 0x01,     // USAGE_PAGE (Generic Desktop)
      0x09, 0x02,     // USAGE (Mouse)
      0xa1, 0x01,     // COLLECTION (Application)
      0x09, 0x01,     //   USAGE (Pointer)
      0xa1, 0x00,     //   COLLECTION (Physical)
      0x05, 0x09,     //     USAGE_PAGE (Button)
      0x19, 0x01,     //     USAGE_MINIMUM (Button 1)
      0x29, 0x03,     //     USAGE_MAXIMUM (Button 3)
      0x15, 0x00,     //     LOGICAL_MINIMUM (0)
      0x25, 0x01,     //     LOGICAL_MAXIMUM (1)
      0x95, 0x03,     //     REPORT_COUNT (3)
      0x75, 0x01,     //     REPORT_SIZE (1)
      0x81, 0x02,     //     INPUT (Data,Var,Abs)
      0x95, 0x01,     //     REPORT_COUNT (1)
      0x75, 0x05,     //     REPORT_SIZE (5)
      0x81, 0x03,     //     INPUT (Cnst,Var,Abs)
      0x05, 0x01,     //     USAGE_PAGE (Generic Desktop)
      0x09, 0x30,     //     USAGE (X)
      0x09, 0x31,     //     USAGE (Y)
      0x15, 0x81,     //     LOGICAL_MINIMUM (-127)
      0x25, 0x7f,     //     LOGICAL_MAXIMUM (127)
      0x75, 0x08,     //     REPORT_SIZE (8)
      0x95, 0x02,     //     REPORT_COUNT (2)
      0x81, 0x06,     //     INPUT (Data,Var,Rel)
      0xc0,           //   END_COLLECTION
      0xc0            // END_COLLECTION
    };
  }
}


#endif
