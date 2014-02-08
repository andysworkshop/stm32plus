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
   * RTC feature to enable access to the second
   * interrupt functionality
   */

  DECLARE_EVENT_SIGNATURE(RtcSecondInterrupt,void());

  class RtcSecondInterruptFeature : public RtcFeatureBase {

    public:
      static RtcSecondInterruptFeature *_secondInterruptFeature;
      void (*_irqRef)();

      DECLARE_EVENT_SOURCE(RtcSecondInterrupt);

    public:
      RtcSecondInterruptFeature(RtcBase& rtc);

      void enableSecondInterrupt() const;
      void disableSecondInterrupt() const;
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcSecondInterruptFeature::RtcSecondInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    _secondInterruptFeature=this;
    _irqRef=RTC_IRQHandler;

    Nvic::configureIrq(RTC_IRQn);
  }


  /**
   * Set the second interrupt
   */

  inline void RtcSecondInterruptFeature::enableSecondInterrupt() const {
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC,ENABLE);
    RTC_WaitForLastTask();
  }


  /**
   * Cancel the second interupt
   */

  inline void RtcSecondInterruptFeature::disableSecondInterrupt() const {
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC,DISABLE);
    RTC_WaitForLastTask();
  }
}
