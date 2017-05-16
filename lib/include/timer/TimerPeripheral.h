/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

 /**
  * The level of remapping required (none, partial1, partial2, full)
  * Not all remap levels are available for each timer. check the ST reference manual for details
  */

  enum TimerGpioRemapLevel {
    TIMER_REMAP_NONE     = 0,
    TIMER_REMAP_PARTIAL1 = 1,
    TIMER_REMAP_PARTIAL2 = 2,
    TIMER_REMAP_FULL     = 3
  };


  /**
   * Generic timer functionality
   * @tparam TTimer The timer class type (Timer1, Timer2...)
   * @tparam TPeripheralName the peripheral class for the clocks
   */

  template<class TTimer,PeripheralName TPeripheralName>
  class TimerPeripheral : public Timer {

    protected:
      TimerPeripheral(TIM_TypeDef *peripheralAddress);
      ~TimerPeripheral();

    public:
      void setPrescalerImmediately(uint16_t value);
      void setPrescalerAtUpdate(uint16_t value);
  };


  /**
   * Constructor
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline TimerPeripheral<TTimer,TPeripheralName>::TimerPeripheral(TIM_TypeDef *peripheralAddress)
    : Timer(peripheralAddress) {

    // set the counter max (32 or 16-bit)

    _counterMax=PeripheralTraits<TPeripheralName>::COUNTER_MAX;

    // enable the clock before the feature constructors are called

    ClockControl<TPeripheralName>::On();
  }


  /**
   * Destructor
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline TimerPeripheral<TTimer,TPeripheralName>::~TimerPeripheral() {

    // disable the clock

    ClockControl<TPeripheralName>::Off();
  }


  /**
   * Set the prescaler for this timer immediately
   * @param value the prescaler
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline void TimerPeripheral<TTimer,TPeripheralName>::setPrescalerImmediately(uint16_t value) {
    TIM_PrescalerConfig(*this,value,TIM_PSCReloadMode_Immediate);
  }


  /**
   * Set the prescaler for this timer at the update event
   * @param value the prescaler
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline void TimerPeripheral<TTimer,TPeripheralName>::setPrescalerAtUpdate(uint16_t value) {
    TIM_PrescalerConfig(*this,value,TIM_PSCReloadMode_Update);
  }
}
