/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Sent to request the capacity in terms of block size and number of blocks. Receiver
     * should set the size and count if the disk is online. If no disk is ready then set
     * read to false, otherwise set to true.
     */

    struct MscBotGetCapacityEvent : UsbEventDescriptor {

      uint8_t lun;
      uint16_t blockSize;
      uint32_t blockCount;
      bool ready;

      MscBotGetCapacityEvent(uint8_t l)
        : UsbEventDescriptor(EventType::MSC_BOT_GET_CAPACITY),
          lun(l) {
      }
    };
  }
}

