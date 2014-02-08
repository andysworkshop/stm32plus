/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

ImageTransitionAnimator::ImageTransitionAnimator(LcdManager::LcdAccess& lcd,BlockDevice& bd)
  : _lcd(lcd),
    _blockDevice(bd) {

  // set a 100Khz timer to tick at 2ms intervals

  _timer.setTimeBaseByFrequency(100000,199);
  _timer.enableInterrupts(TIM_IT_Update);
  _timer.TimerInterruptEventSender.insertSubscriber(
      TimerInterruptEventSourceSlot::bind(this,&ImageTransitionAnimator::onTimerInterrupt)
    );

  // initialise the easing function

  _ease.setDuration(TRANSITION_TIME);
  _ease.setTotalChangeInPosition(320);

  // preset for no previous image

  _blockIndex=0;
}


/**
 * Set the position of the new image block index
 * @param blockIndex where the new image starts
 */

void ImageTransitionAnimator::setNewImageBlockIndex(uint32_t blockIndex) {
  _lastBlockIndex=_blockIndex;
  _blockIndex=blockIndex;
}


/*
 * Start the transition and wait until it's done. The transition is synchronised to the tick of
 * the UPDATE event of the timer. Each update event moves the animation onwards by one position.
 */

void ImageTransitionAnimator::start() {

  int16_t newPosition,count;
  uint32_t currentTime,i,lastBlock,firstBlock;
  uint16_t blocks[512];
  int16_t currentScrollPosition;

  _ticker=0;
  _timer.enablePeripheral();

  currentScrollPosition=0;
  currentTime=0;

  while(currentTime<TRANSITION_TIME) {

    currentTime=_ticker<=TRANSITION_TIME ? _ticker : TRANSITION_TIME;

    // scroll the display

    newPosition=_ease.easeOut(currentTime);
    count=newPosition-currentScrollPosition;

    if(count!=0) {

      _lcd.setScrollPosition(newPosition);

      // blit in the new data

      _lcd.moveToPoint(Point(0,currentScrollPosition));

      firstBlock=_blockIndex+(currentScrollPosition*2);
      lastBlock=_blockIndex+(currentScrollPosition*2)+(count*2);

      currentScrollPosition=newPosition;

      _lcd.beginWriting();

      for(i=firstBlock;i<lastBlock;i+=2) {

        // read the 2 blocks (one scan)

        if(!_blockDevice.readBlocks(blocks,i,2))
          return;

        // write to the device

        _lcd.rawTransfer(blocks,120);                       // first 120 pixels (480 bytes) are here
        _lcd.rawTransfer(&blocks[256],120);                 // next 120 pixels are in the next sector
      }
    }
  }
}


/*
 * Interrupt callback function. This is called when the update interrupt that we've
 * enabled is fired.
 */

void ImageTransitionAnimator::onTimerInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

  if(tet==TimerEventType::EVENT_UPDATE)
    _ticker++;
}

