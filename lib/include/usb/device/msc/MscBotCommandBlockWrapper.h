/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Command block wrapper (CBW) structure
     */

    struct MscBotCommandBlockWrapper {
      uint32_t dSignature;
      uint32_t dTag;
      uint32_t dDataLength;
      uint8_t bmFlags;
      uint8_t bLUN;
      uint8_t bCBLength;
      uint8_t CB[16];
    } __attribute__((packed));
  }
}
