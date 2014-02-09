/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "stdafx.h"


/*
 * Run the app
 */

void PictureFrame::run() {

  if(!initialise())
    return;

  processEvents();
}


/*
 * Initialise
 */

bool PictureFrame::initialise() {

  // initialise Nvic priority group configuration

  Nvic::initialise();

  // initialise the millisecond timer

  MillisecondTimer::initialise();

  // default to 20 seconds

  _autoScrollMillis=20000;
  _autoScrollEnabled=true;

  // create and init the LCD manager

  _lcdManager=new LcdManager;
  if(!_lcdManager->initialise())
    return false;

  // create and init the FS manager

  _fsManager=new FileSystemManager(*_lcdManager);
  if(!_fsManager->initialise())
    return false;

  // create and init the touch manager

  _touchManager=new TouchManager(*_lcdManager,*_fsManager);
  if(!_touchManager->initialise())
    return false;

  // create and init the bitmap manager

  _bitmapManager=new BitmapManager(*_lcdManager,*_fsManager);
  if(!_bitmapManager->initialise())
    return false;

  // create the thumb manager

  _thumbManager=new ThumbManager(_lcdManager->getLcd(),*_fsManager,*_touchManager,_bitmapManager->getImageCount());

  // if the touch screen needs calibration, do it

  if(_touchManager->needsCalibration() && !_touchManager->calibrate())
    return false;

  // clear the screen

  _lcdManager->getLcd().clearScreen();

  // create the image manager

  _imageManager=new ImageManager(*_lcdManager,*_bitmapManager,_fsManager->getSdCard());
  return _imageManager->initialise();
}


/*
 * Process events
 */

void PictureFrame::processEvents() {

  Point p;
  uint32_t last,now;

  last=MillisecondTimer::millis();

  for(;;) {

    now=MillisecondTimer::millis();

    // test for a wrap on the timer

    if(last>now) {
      last=now;
      continue;
    }

    // time to scroll?

    if(_autoScrollEnabled && now-last>_autoScrollMillis) {
      _imageManager->nextImage();
      last=now;
    }

    // process clicks

    if(processClick())
      last=MillisecondTimer::millis();
  }
}


/*
 * Detect and process a click
 */

bool PictureFrame::processClick() {

  uint32_t nextImage;

  // check for click

  if(!_touchManager->clicked())
    return false;

  // last touch must come up

  _touchManager->waitForPenUp();

  // display UI and get the next image from it

  if(_thumbManager->run(nextImage,_autoScrollMillis,_autoScrollEnabled))
    _imageManager->setCurrentImage(nextImage);

  _touchManager->waitForPenUp();

  // show the image

  _imageManager->showImage();
  return true;
}
