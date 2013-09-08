/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Class to manage the transition between images. Uses an easing function
 * and the native scrolling ability of the LCD.
 */

class ImageTransitionAnimator {

  protected:
    static const uint32_t TRANSITION_TIME=600;

    Timer3<Timer3InternalClockFeature,Timer3InterruptFeature> _timer;

    LcdManager::LcdAccess& _lcd;
    BlockDevice& _blockDevice;
    uint32_t _blockIndex;
    uint32_t _lastBlockIndex;
    BounceEase _ease;

    volatile uint32_t _ticker;

  public:
    ImageTransitionAnimator(LcdManager::LcdAccess& lcd,BlockDevice& bd);
    virtual ~ImageTransitionAnimator() {}

    void setNewImageBlockIndex(uint32_t blockIndex);
    void start();

    // overrides from timer Observer
    void onTimerInterrupt(TimerEventType tet,uint8_t timerNumber);
};
