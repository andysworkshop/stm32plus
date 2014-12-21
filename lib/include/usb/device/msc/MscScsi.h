/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * SCSI structure
     */

    struct MscScsi {

      enum {
        SENSE_LIST_DEPTH = 4
      };

      MscScsiSense sense[SENSE_LIST_DEPTH];
      uint8_t senseHead;
      uint8_t senseTail;

      uint16_t blkSize;
      uint32_t blkNbr;

      uint32_t blkAddr;
      uint32_t blkLen;
    };
  }
}
