/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


/*
 * Forward declare the IRQ handler names
 */

extern "C" void RTC_IRQHandler();


namespace stm32plus {


  /**
   * RTC feature to enable access to the overflow
   * interrupt functionality
   */

  DECLARE_EVENT_SIGNATURE(RtcOverflowInterrupt,void());

  class RtcOverflowInterruptFeature : public RtcFeatureBase {

    public:
      static RtcOverflowInterruptFeature *_overflowInterruptFeature;
      void (*_irqRef)();

      DECLARE_EVENT_SOURCE(RtcOverflowInterrupt);

    public:
      RtcOverflowInterruptFeature(RtcBase& rtc);

      void enableOverflowInterrupt() const;
      void disableOverflowInterrupt() const;
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcOverflowInterruptFeature::RtcOverflowInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    _overflowInterruptFeature=this;
    _irqRef=RTC_IRQHandler;

    Nvic::configureIrq(RTC_IRQn);
  }


  /**
   * Set the second interrupt
   */

  inline void RtcOverflowInterruptFeature::enableOverflowInterrupt() const {
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_OW,ENABLE);
    RTC_WaitForLastTask();
  }


  /**
   * Cancel the second interupt
   */

  inline void RtcOverflowInterruptFeature::disableOverflowInterrupt() const {
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_OW,DISABLE);
    RTC_WaitForLastTask();
  }
}
