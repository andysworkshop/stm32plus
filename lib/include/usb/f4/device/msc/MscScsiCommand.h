/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * SCSI commands
     */

    enum class MscScsiCommand : uint8_t {
      FORMAT_UNIT            = 0x04,
      INQUIRY                = 0x12,
      MODE_SELECT6           = 0x15,
      MODE_SELECT10          = 0x55,
      MODE_SENSE6            = 0x1A,
      MODE_SENSE10           = 0x5A,
      ALLOW_MEDIUM_REMOVAL   = 0x1E,
      READ6                  = 0x08,
      READ10                 = 0x28,
      READ12                 = 0xA8,
      READ16                 = 0x88,

      READ_CAPACITY10        = 0x25,
      READ_CAPACITY16        = 0x9E,

      REQUEST_SENSE          = 0x03,
      START_STOP_UNIT        = 0x1B,
      TEST_UNIT_READY        = 0x00,
      WRITE6                 = 0x0A,
      WRITE10                = 0x2A,
      WRITE12                = 0xAA,
      WRITE16                = 0x8A,

      VERIFY10               = 0x2F,
      VERIFY12               = 0xAF,
      VERIFY16               = 0x8F,

      SEND_DIAGNOSTIC        = 0x1D,
      READ_FORMAT_CAPACITIES = 0x23
    };
  }
}
