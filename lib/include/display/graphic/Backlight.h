/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * The backlight uses a timer class that must have been declared with a TimerPwmFeature
     */

    template<class TTimer>
    class Backlight : public TTimer {

      public:
        Backlight(uint32_t frequency=1000000);

      public:
        void fadeTo(uint16_t newPercentage,int msPerStep);
    };


    /**
     * Constructor. set the timebase
     */

    template<class TTimer>
    inline Backlight<TTimer>::Backlight(uint32_t frequency) {

      // set the timebase for PWM to user's frequency choice (default 1Mhz)

      this->setTimeBaseByFrequency(frequency,2000);

      // initialise the timer's comparator (must have been declared with a channel feature)

      this->initCompareForPwmOutput();

      // enable the timer

      this->Timer::enablePeripheral();
    }


    /**
     * fade up or down to the supplied percentage waiting
     * for msPerStep millis between each step
     * @param newPercentage 0..100
     * @param msPerStep Milliseconds to pause between each step
     */

    template<class TTimer>
    inline void Backlight<TTimer>::fadeTo(uint16_t newPercentage,int msPerStep) {

      int8_t direction;

      if(newPercentage==this->_dutyCycle)
        return;

      direction=newPercentage>this->_dutyCycle ? 1 : -1;

      while(newPercentage!=this->_dutyCycle) {
        this->setDutyCycle(this->_dutyCycle+direction);
        MillisecondTimer::delay(msPerStep);
      }
    }


#if defined(STM32PLUS_F0)

    /**
     * On the F0 we'll default the backlight to PA7 (TIM17, CH1)
     */

    typedef Backlight<Timer17<Timer17InternalClockFeature,
                             TimerChannel1Feature<>,
                             Timer17GpioFeature<TIMER_REMAP_NONE,TIM17_CH1_OUT>
                             > > DefaultBacklight;
#else

    /**
     * For compatibility with earlier versions, default backlight is
     * Timer4, channel2, remapped GPIO. (PD13)
     */

    typedef Backlight<Timer4<Timer4InternalClockFeature,
                             TimerChannel2Feature<>,
                             Timer4GpioFeature<TIMER_REMAP_FULL,TIM4_CH2_OUT>
                             > > DefaultBacklight;

#endif
  }
}
