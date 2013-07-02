/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Class to manage the file system
 */

class FileSystemManager : public Initialiser {

  protected:
    FileSystem *_fs;
    SdioDmaSdCard *_sdcard;
    CachedBlockDevice *_cachedBlockDevice;
    uint32_t _firstCacheBlock;
    bool _imagesAreCached;

  protected:
    void readImageCacheStatus();

  public:
    FileSystemManager(LcdManager& lcdManager);

    bool allocateBlocks(uint32_t imageCount);

    FileSystem& getFileSystem() const {
      return *_fs;
    }

    BlockDevice& getSdCard() const {
      return *_sdcard;
    }

    uint32_t getFirstCacheBlock() const {
      return _firstCacheBlock;
    }

    bool imagesAreCached() const {
      return _imagesAreCached;
    }

    // overrides from Initialiser

    virtual bool initialise();
};
