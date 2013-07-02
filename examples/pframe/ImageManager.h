/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Image manager class
 */

class ImageManager : public Initialiser {

  protected:
    BitmapManager& _bmManager;
    BlockDevice& _blockDevice;
    uint32_t _currentImage;
    ImageTransitionAnimator _imageTransition;

  protected:
    bool drawFullImage();

  public:
    ImageManager(LcdManager& lcdManager,BitmapManager& bmManager,BlockDevice& blockDevice);
    virtual ~ImageManager() {}

    ImageTransitionAnimator& getAnimator();

    LcdManager& getLcdManager() {
      return _lcdManager;
    }

    void setCurrentImage(uint32_t imageIndex) {
      _currentImage=imageIndex;
    }

    void nextImage();
    void showImage();

    // overrides from Initialiser

    bool initialise();
};
