/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Advanced control timers are TIM1, TIM8
   * @tparam TTimer The timer class type (Timer1, Timer2...)
   * @tparam TPeripheralName the peripheral class for the clocks
   */

  template<class TTimer,PeripheralName TPeripheralName>
  class AdvancedControlTimer : public TimerPeripheral<TTimer,TPeripheralName> {

    protected:
      AdvancedControlTimer(TIM_TypeDef *peripheralAddress);

    public:
      void initialiseTimeBaseWithRepeat(uint16_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode,uint8_t repeatCount);
      void setTimeBaseByFrequencyWithRepeat(uint32_t frequency,uint16_t arr,uint16_t counterMode,uint8_t repeatCount);
  };


  /**
   * Constructor
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline AdvancedControlTimer<TTimer,TPeripheralName>::AdvancedControlTimer(TIM_TypeDef *peripheralAddress)
    : TimerPeripheral<TTimer,TPeripheralName>(peripheralAddress) {
  }


  /**
   * Initialise the time base for this timer
   * @param period Configures the period value to be loaded into the active Auto-Reload Register at the next update event.
   * @param prescaler Configures the prescaler value used to divide the TIM clock.
   * @param clockDivision TIM_CKD_DIV1/2/4
   * @param counterMode TIM_CounterMode_Up/Down
   * @param repeatCount The number of times that a counter must repeat its end-to-end cycle before generating Update.
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline void AdvancedControlTimer<TTimer,TPeripheralName>::initialiseTimeBaseWithRepeat(uint16_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode,uint8_t repeatCount) {

    // repeat count is unique to the advanced control timers. Set the value and call the
    // base to complete the init.

    this->_timeBase.TIM_RepetitionCounter=repeatCount;
    Timer::initialiseTimeBase(period,prescaler,clockDivision,counterMode);
  }


  /**
   * Convenience function to set the timer clock (TIMxCLK) to the desired frequency. The counter will
   * be an up counter (by default) with a period equal to the arr (auto-reload) value. The lowest
   * frequency that can be set is TIMxCLK / 65536. For a 72Mhz core clock this is 1098 Hz.
   * @param frequency The frequency in Hz.
   * @param arr The auto reload value (0..65535). The timer counter reverses/resets at this value.
   * @param counterMode TIM_CounterMode_* value
   * @param repeatCount The number of times that a counter must repeat its end-to-end cycle before generating Update.
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline void AdvancedControlTimer<TTimer,TPeripheralName>::setTimeBaseByFrequencyWithRepeat(uint32_t frequency,uint16_t arr,uint16_t counterMode,uint8_t repeatCount) {

    this->_timeBase.TIM_RepetitionCounter=repeatCount;
    Timer::initialiseTimeBase(arr,(this->_clock/frequency)-1,0,counterMode);
  }
}
