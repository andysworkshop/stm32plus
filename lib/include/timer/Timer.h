/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Base class for timer implementations.
   *
   * The timer is observable for interrupts when the implementation has enabled them
   */

  class Timer {

    protected:
      TIM_TypeDef *_peripheralAddress;
      TIM_TimeBaseInitTypeDef _timeBase;
      uint32_t _clock;
      uint32_t _counterMax;

    protected:
      Timer(TIM_TypeDef *peripheralAddress);

    public:
      operator TIM_TypeDef *();
      operator TIM_TimeBaseInitTypeDef *();

      void enablePeripheral() const;
      void disablePeripheral() const;

      uint32_t getPeriod() const;
      uint32_t getClock() const;
      uint32_t getCounterMax() const;
      uint32_t getCounter() const;

      void setClock(uint32_t clock);
      void setCounter(uint32_t counter) const;

      void setPrescaler(uint16_t prescaler,uint16_t reloadMode=TIM_PSCReloadMode_Immediate) const;
      void initialiseTimeBase(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode);
      void setTimeBaseByFrequency(uint32_t frequency,uint32_t arr,uint16_t counterMode=TIM_CounterMode_Up);
      void deinitialise();
  };


  /*
   * Constructor
   */

  inline Timer::Timer(TIM_TypeDef *peripheralAddress)
    : _peripheralAddress(peripheralAddress) {

    // clear this down ready for the initialisation that must come shortly after

    TIM_TimeBaseStructInit(&_timeBase);
  }


  /**
   * Initialise the time base for this timer
   * @param period Configures the period value to be loaded into the active Auto-Reload Register at the next update event.
   * @param prescaler Configures the prescaler value used to divide the TIM clock.
   * @param clockDivision TIM_CKD_DIV1/2/4
   * @param counterMode TIM_CounterMode_Up/Down
   */

  inline void Timer::initialiseTimeBase(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode) {

    _timeBase.TIM_Period=period;
    _timeBase.TIM_Prescaler=prescaler;
    _timeBase.TIM_ClockDivision=clockDivision;
    _timeBase.TIM_CounterMode=counterMode;

    TIM_TimeBaseInit(_peripheralAddress,&_timeBase);
  }


  /**
   * Convenience function to set the timer clock (TIMxCLK) to the desired frequency. The counter will
   * be an up counter (by default) with a period equal to the arr (auto-reload) value. The lowest
   * frequency that can be set is TIMxCLK / 65536. For a 72Mhz core clock this is 1098 Hz.
   * @param frequency The frequency in Hz.
   * @param arr The auto reload value (0..0xFFFF/0xFFFFFFFF). The timer counter reverses/resets at this value.
   * @param counterMode TIM_CounterMode_* value
   */

  inline void Timer::setTimeBaseByFrequency(uint32_t frequency,uint32_t arr,uint16_t counterMode) {

    initialiseTimeBase(arr,(_clock/frequency)-1,0,counterMode);
  }


  /**
   * Get the maximum value of the timer counter. For 32-bit timers (TIM2/5 on the F4 and TIM2
   * on the F0) this will be 0xFFFFFFFF and for all others it's 0xFFFF
   * @return The maximum counter value
   */

  inline uint32_t Timer::getCounterMax() const {
    return _counterMax;
  }


  /**
   * Return the actual period (the value loaded into the reload register is this period-1)
   * @return The period
   */

  inline uint32_t Timer::getPeriod() const {
    return _timeBase.TIM_Period;
  }


  /**
   * Get the clock value for this timer. For internally clocked APB1 peripherals this is HCLK/2, for
   * internally clocked APB2 peripherals this is HCLK
   * @return The clock frequency in Hz
   */

  inline uint32_t Timer::getClock() const {
    return _clock;
  }


  /**
   * Get the current value of the counter
   * @return The counter value
   */

  inline uint32_t Timer::getCounter() const {
    return TIM_GetCounter(_peripheralAddress);
  }


  /**
   * Get the clock value for this timer. For internally clocked APB1 peripherals this is HCLK/2, for
   * internally clocked APB2 peripherals this is HCLK
   * @param clock The clock frequency in Hz
   */

  inline void Timer::setClock(uint32_t clock) {
    _clock=clock;
  }


  /**
   * Reset the counter to a known value. Useful when not counting automatically, e.g.
   * on an external input.
   * @param counter The new value for the counter
   */

  inline void Timer::setCounter(uint32_t counter) const {
    TIM_SetCounter(_peripheralAddress,counter);
  }


  /**
   * Return the peripheral address
   * @return TIM_TypeDef * peripheral address
   */

  inline Timer::operator TIM_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Return the timebase initialisation structure
   */

  inline Timer::operator TIM_TimeBaseInitTypeDef *() {
    return &_timeBase;
  }


  /**
   * Enable this timer
   */

  inline void Timer::enablePeripheral() const {
    TIM_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable this timer
   */

  inline void Timer::disablePeripheral() const {
    TIM_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Set the timer prescaler
   * @param prescaler prescaler value
   * @param reloadMode TIM_PSCReloadMode value (TIM_PSCReloadMode_Update/TIM_PSCReloadMode_Immediate). Default is immediate.
   */

  inline void Timer::setPrescaler(uint16_t prescaler,uint16_t reloadMode) const {
    TIM_PrescalerConfig(_peripheralAddress,prescaler,reloadMode);
  }


  /**
   * De-initialise the timer completely
   */

  inline void Timer::deinitialise() {
    TIM_DeInit(_peripheralAddress);
  }
}
