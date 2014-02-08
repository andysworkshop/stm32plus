/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * An input stream class that reads from a block device.
   */

  class BlockDeviceInputStream : public InputStream {

    protected:
      BlockDevice& _device;
      ByteMemblock _block;
      uint32_t _originalBlockIndex;
      uint32_t _blockIndex;
      uint32_t _indexInBlock;

    protected:
      int16_t requireBytes();

    public:
      enum {
        E_INVALID_SEEK_POSITION = 1
      };

    public:
      BlockDeviceInputStream(BlockDevice& device,uint32_t firstBlock);
      virtual ~BlockDeviceInputStream() {}

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool close() override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;
  };
}
