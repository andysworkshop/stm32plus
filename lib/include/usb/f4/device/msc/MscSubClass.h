/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * MSC subclass codes
     */

    enum class MscSubClass : uint8_t {
      NOT_REPORTED = 0,
      RBC = 1,
      ATAPI = 2,
      UFI = 4,
      SCSI_TRANSPARENT = 6,
      LSDFS = 7,
      IEEE1667 = 8,
      VENDOR = 0xff
    };
  }
}
