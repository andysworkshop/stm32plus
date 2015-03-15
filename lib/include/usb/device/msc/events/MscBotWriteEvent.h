/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Write data to the LUN. Block address is a block index, not a byte address.
     */

    struct MscBotWriteEvent : UsbEventDescriptor {

      bool success;
      uint8_t lun;
      const uint8_t *buffer;
      uint32_t blockAddress;
      uint16_t blockCount;

      MscBotWriteEvent(uint8_t l,const uint8_t *buf,uint32_t blk_addr,uint16_t blk_len)
        : UsbEventDescriptor(EventType::MSC_BOT_WRITE),
          lun(l),
          buffer(buf),
          blockAddress(blk_addr),
          blockCount(blk_len) {
      }
    };
  }
}

