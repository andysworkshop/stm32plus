/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * General purpose timers on the F1 are TIM2..5
   * @tparam TTimer The timer class type (Timer1, Timer2...)
   */

  template<class TTimer,PeripheralName TPeripheralName>
  class BasicTimer : public TimerPeripheral<TTimer,TPeripheralName> {

    protected:
      BasicTimer(TIM_TypeDef *peripheralAddress);
 };


  /**
   * Constructor
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline BasicTimer<TTimer,TPeripheralName>::BasicTimer(TIM_TypeDef *peripheralAddress)
    : TimerPeripheral<TTimer,TPeripheralName>(peripheralAddress) {
  }
}
