/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * General purpose timers are TIM2..5, 9..14
   * @tparam TTimer The timer class type (Timer1, Timer2...)
   */

  template<class TTimer,PeripheralName TPeripheralName>
  class GeneralPurposeTimer : public TimerPeripheral<TTimer,TPeripheralName> {

    protected:
      GeneralPurposeTimer(TIM_TypeDef *peripheralAddress);
 };


  /**
   * Constructor
   */

  template<class TTimer,PeripheralName TPeripheralName>
  inline GeneralPurposeTimer<TTimer,TPeripheralName>::GeneralPurposeTimer(TIM_TypeDef *peripheralAddress)
    : TimerPeripheral<TTimer,TPeripheralName>(peripheralAddress) {
  }
}
