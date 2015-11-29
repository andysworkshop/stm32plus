/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Read data from the LUN. Block address is a block index, not a byte address.
     */

    struct MscBotReadEvent : UsbEventDescriptor {

      bool success;
      uint8_t lun;
      uint8_t *buffer;
      uint32_t blockAddress;
      uint16_t blockCount;

      MscBotReadEvent(uint8_t l,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len)
        : UsbEventDescriptor(EventType::MSC_BOT_READ),
          lun(l),
          buffer(buf),
          blockAddress(blk_addr),
          blockCount(blk_len) {
      }
    };
  }
}

