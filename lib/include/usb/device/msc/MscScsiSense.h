/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * SCSI sense structure
     */

    struct MscScsiSense {

      enum {
        NO_SENSE        = 0,
        RECOVERED_ERROR = 1,
        NOT_READY       = 2,
        MEDIUM_ERROR    = 3,
        HARDWARE_ERROR  = 4,
        ILLEGAL_REQUEST = 5,
        UNIT_ATTENTION  = 6,
        DATA_PROTECT    = 7,
        BLANK_CHECK     = 8,
        VENDOR_SPECIFIC = 9,
        COPY_ABORTED    = 10,
        ABORTED_COMMAND = 11,
        VOLUME_OVERFLOW = 13,
        MISCOMPARE      = 14
      };

      enum {
        INVALID_CDB                     = 0x20,
        INVALID_FIELED_IN_COMMAND       = 0x24,
        PARAMETER_LIST_LENGTH_ERROR     = 0x1A,
        INVALID_FIELD_IN_PARAMETER_LIST = 0x26,
        ADDRESS_OUT_OF_RANGE            = 0x21,
        MEDIUM_NOT_PRESENT              = 0x3A,
        MEDIUM_HAVE_CHANGED             = 0x28,
        WRITE_PROTECTED                 = 0x27,
        UNRECOVERED_READ_ERROR          = 0x11,
        WRITE_FAULT                     = 0x03
      };

      char Skey;
      union {
        struct _ASCs {
          char ASC;
          char ASCQ;
        } b;
        unsigned int ASC;
        char *pData;
      } w;
    };
  }
}
