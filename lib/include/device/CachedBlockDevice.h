/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * @brief Specialisation of BlockDevice to provide in-memory caching.
   *
   * This class subclasses BlockDevice and provides in-memory caching of recently
   * used blocks. The cache is write-through. A user of this class must ensure
   * that he has enough RAM in his device to support the desired cache size.
   */

  class CachedBlockDevice : public BlockDevice {

    protected:

      struct CacheEntry {
          uint32_t BlockIndex;
          uint32_t CacheIndex;
      };

      BlockDevice& _device;
      uint32_t _numCachedBlocks;
      uint8_t **_cachedBlocks;
      CacheEntry *_cacheIndex;
      uint32_t _blockSize;

      static constexpr uint32_t FREE_CACHE_ENTRY=0xFFFFFFFF;

    protected:
      void writeToCache(const void *data,uint32_t blockIndex);
      void moveToFront(uint32_t index);

    public:
      CachedBlockDevice(BlockDevice& bd,uint32_t numCachedBlocks);
      virtual ~CachedBlockDevice();

      // overrides from BlockDevice

      virtual uint32_t getBlockSizeInBytes() override;

      virtual bool readBlock(void *dest,uint32_t blockIndex) override;
      virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) override;

      virtual bool writeBlock(const void *src,uint32_t blockIndex) override;
      virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) override;

      virtual uint32_t getTotalBlocksOnDevice() override;

      virtual formatType getFormatType() override;
  };
}

