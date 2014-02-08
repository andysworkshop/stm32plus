/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * An output stream class that writes to a block device. Optionally
   * writes back for every write or only when a block is full
   */

  class BlockDeviceOutputStream : public OutputStream {

    protected:
      BlockDevice& _device;
      ByteMemblock _block;
      uint32_t _blockIndex;
      uint32_t _indexInBlock;
      bool _buffered;

    protected:
      bool checkFillBuffer(uint32_t toWrite);
      bool checkFlush();

    public:
      /**
       * Error codes
       */

      enum {
        /// We've run out of blocks
        E_DEVICE_FULL=1
      };

    public:
      BlockDeviceOutputStream(BlockDevice& device,uint32_t firstBlock,bool buffered);
      virtual ~BlockDeviceOutputStream();

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool close() override;
      virtual bool flush() override;
  };
}
