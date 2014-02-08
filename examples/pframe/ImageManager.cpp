/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

ImageManager::ImageManager(LcdManager& lcdManager,BitmapManager& bmManager,BlockDevice& bd)
  : Initialiser(lcdManager),
    _bmManager(bmManager),
    _blockDevice(bd),
    _imageTransition(_lcdManager.getLcd(),bd) {
}


/*
 * Initialise the class
 */

bool ImageManager::initialise()
{
  _currentImage=0;

  return drawFullImage();
}


/*
 * Draw the current image in full
 */

bool ImageManager::drawFullImage() {

  uint32_t blockIndex,i;
  uint16_t blocks[512];

  // get the first block. There are two blocks per scan line.

  blockIndex=_bmManager.getFirstBlockIndex(_currentImage);

  // get the display device

  LcdManager::LcdAccess& lcd=_lcdManager.getLcd();

  lcd.moveTo(lcd.getFullScreenRectangle());
  lcd.beginWriting();

  for(i=blockIndex;i<blockIndex+640;i+=2) {

    // read the 2 blocks (one scan)

    if(!_blockDevice.readBlocks(blocks,i,2))
      return false;

    // write to the device

    lcd.rawTransfer(blocks,120);               // 120 pixels are here (480 bytes in one 512b sector)
    lcd.rawTransfer(&blocks[256],120);         // and the next 120 are here
  }

  return true;
}


/*
 * Move to the next image
 */

void ImageManager::nextImage() {

  // advance and wrap if we have to

  if(_currentImage==_bmManager.getImageCount()-1)
    _currentImage=0;
  else
    _currentImage++;

  // show the current image

  showImage();
}


/*
 * Show image with animated transition
 */

void ImageManager::showImage() {

  // set the new image

  _imageTransition.setNewImageBlockIndex(_bmManager.getFirstBlockIndex(_currentImage));

  // start the timeline

  _imageTransition.start();
}


/*
 * Get the animator
 */

ImageTransitionAnimator& ImageManager::getAnimator() {
  return _imageTransition;
}
