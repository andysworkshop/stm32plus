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
     * The keyboard report descriptor bytes
     */


    const uint8_t KeyboardReportDescriptor[63] = {
      0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
      0x09, 0x06,                    // USAGE (Keyboard)
      0xa1, 0x01,                    // COLLECTION (Application)
      0x75, 0x01,                    //   REPORT_SIZE (1)
      0x95, 0x08,                    //   REPORT_COUNT (8)
      0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
      0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)(224)
      0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
      0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
      0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
      0x81, 0x02,                    //   INPUT (Data,Var,Abs) ; Modifier byte
      0x95, 0x01,                    //   REPORT_COUNT (1)
      0x75, 0x08,                    //   REPORT_SIZE (8)
      0x81, 0x03,                    //   INPUT (Cnst,Var,Abs) ; Reserved byte
      0x95, 0x05,                    //   REPORT_COUNT (5)
      0x75, 0x01,                    //   REPORT_SIZE (1)
      0x05, 0x08,                    //   USAGE_PAGE (LEDs)
      0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
      0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
      0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) ; LED report
      0x95, 0x01,                    //   REPORT_COUNT (1)
      0x75, 0x03,                    //   REPORT_SIZE (3)
      0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs) ; LED report padding
      0x95, 0x06,                    //   REPORT_COUNT (6)
      0x75, 0x08,                    //   REPORT_SIZE (8)
      0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
      0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
      0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
      0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))(0)
      0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)(101)
      0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
      0xc0                           // END_COLLECTION
    };
  }
}


#endif
