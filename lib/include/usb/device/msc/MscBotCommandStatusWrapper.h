/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Command status wrapper (CSW) structure
     */

    struct MscBotCommandStatusWrapper {

      uint32_t dSignature;
      uint32_t dTag;
      uint32_t dDataResidue;
      MscBotCswStatus bStatus;

      MscBotCommandStatusWrapper() {
        static_assert(sizeof(MscBotCommandStatusWrapper)==13,"Compiler error: sizeof(MscBotCommandStatusWrapper)!=13");
        dSignature=0x53425355;
      }
    } __attribute__((packed));
  }
}
