/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/device.h"


namespace stm32plus {

  /**
   * Constructor
   *
   * @param[in] bd The block device being cached. Must not go out of scope.
   * @param[in] numCachedBlocks The number of blocks to cache. This parameter controls the memory used by this class.
   */

  CachedBlockDevice::CachedBlockDevice(BlockDevice& bd,uint32_t numCachedBlocks) :
    _device(bd), _numCachedBlocks(numCachedBlocks), _blockSize(bd.getBlockSizeInBytes()) {
    uint32_t i;

    _cachedBlocks=new uint8_t *[numCachedBlocks];
    _cacheIndex=new CacheEntry[numCachedBlocks];

    for(i=0;i<numCachedBlocks;i++) {
      _cachedBlocks[i]=new uint8_t[_blockSize];
      _cacheIndex[i].BlockIndex=FREE_CACHE_ENTRY;
      _cacheIndex[i].CacheIndex=i;
    }
  }

  /**
   * Destructor. Free memory allocated by the cache.
   */

  CachedBlockDevice::~CachedBlockDevice() {

    uint32_t i;

    for(i=0;i<_numCachedBlocks;i++)
      delete[] _cachedBlocks[i];

    delete _cachedBlocks;
    delete _cacheIndex;
  }

  /*
   * read a block
   */

  bool CachedBlockDevice::readBlock(void *dest,uint32_t blockIndex) {

    uint32_t i;

    // try to find in the cache index

    for(i=0;i<_numCachedBlocks&&_cacheIndex[i].BlockIndex!=FREE_CACHE_ENTRY;i++) {

      if(_cacheIndex[i].BlockIndex==blockIndex) {

        // cache hit

        memcpy(dest,_cachedBlocks[_cacheIndex[i].CacheIndex],_blockSize);
        moveToFront(i);
        return true;
      }
    }

    // cache miss, read from device

    if(!_device.readBlock(dest,blockIndex))
      return false;

    // write to the cache

    writeToCache(dest,blockIndex);
    return true;
  }

  /*
   * write a block
   */

  bool CachedBlockDevice::writeBlock(const void *src,uint32_t blockIndex) {

    // write through

    if(!_device.writeBlock(src,blockIndex))
      return false;

    // and into the cache

    writeToCache(src,blockIndex);
    return true;
  }

  /*
   * find cache entry and write
   */

  void CachedBlockDevice::writeToCache(const void *data,uint32_t blockIndex) {

    uint32_t i;
    CacheEntry evictedEntry;

    // if already in the cache index then move its entry to the front

    for(i=0;i<_numCachedBlocks&&_cacheIndex[i].BlockIndex!=FREE_CACHE_ENTRY;i++) {

      if(_cacheIndex[i].BlockIndex==blockIndex) {
        moveToFront(i);
        memcpy(_cachedBlocks[_cacheIndex[i].CacheIndex],data,_blockSize);
        return;
      }
    }

    // move up the whole index losing the last one

    evictedEntry=_cacheIndex[_numCachedBlocks-1];
    memmove(&_cacheIndex[1],&_cacheIndex[0],sizeof(CacheEntry)*(_numCachedBlocks-1));

    // this is a new entry into the index so store its data

    memcpy(_cachedBlocks[evictedEntry.CacheIndex],data,_blockSize);

    evictedEntry.BlockIndex=blockIndex;
    _cacheIndex[0]=evictedEntry;
  }

  /*
   * move an entry to the front of the index
   */

  void CachedBlockDevice::moveToFront(uint32_t index) {

    if(index!=0) {

      CacheEntry entry;

      entry=_cacheIndex[index];
      memmove(&_cacheIndex[1],&_cacheIndex[0],sizeof(CacheEntry)*index);
      _cacheIndex[0]=entry;
    }
  }

  /*
   * multi-block write
   */

  bool CachedBlockDevice::writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) {

    uint32_t i;
    const uint8_t *ptr;

    ptr=static_cast<const uint8_t *> (src);

    for(i=0;i<numBlocks;i++) {

      if(!writeBlock(ptr,blockIndex+i))
        return false;

      ptr+=_blockSize;
    }

    return true;
  }

  /*
   * multi-block read
   */

  bool CachedBlockDevice::readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) {
    uint32_t i;
    uint8_t *ptr;

    ptr=static_cast<uint8_t *> (dest);

    for(i=0;i<numBlocks;i++) {

      if(!readBlock(ptr,blockIndex+i))
        return false;

      ptr+=_blockSize;
    }

    return true;
  }

  /*
   * pass through to device
   */

  BlockDevice::formatType CachedBlockDevice::getFormatType() {
    return _device.getFormatType();
  }

  uint32_t CachedBlockDevice::getBlockSizeInBytes() {
    return _blockSize;
  }

  uint32_t CachedBlockDevice::getTotalBlocksOnDevice() {
    return _device.getTotalBlocksOnDevice();
  }
}
