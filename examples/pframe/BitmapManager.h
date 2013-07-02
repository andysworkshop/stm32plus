/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Class to manage bitmap IO
 */

class BitmapManager : public Initialiser {

  protected:
    FileSystemManager& _fsManager;
    uint32_t _imageCount;

    static const uint32_t IMAGE_BYTE_SIZE = 307200;

  protected:
    bool createBitmapBlocks();
    bool readImages();
    bool countImages();

  public:
    BitmapManager(LcdManager& lcdManager,FileSystemManager& fsManager);
    virtual ~BitmapManager() {}

    bool openImage(uint32_t imageIndex,File*& file);

    /*
     * Get the first block index for this image
     */

    uint32_t getFirstBlockIndex(uint32_t imageIndex) const {
      return _fsManager.getFirstCacheBlock()+(imageIndex*640);
    }

    /*
     * Get the count of images
     */

    uint32_t getImageCount() const {
      return _imageCount;
    }

    // overrides from Initialiser

    virtual bool initialise();
};
