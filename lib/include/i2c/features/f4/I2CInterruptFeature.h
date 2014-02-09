/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


/*
 * Forward declare the IRQ handler names
 */

extern "C" void I2C3_EV_IRQHandler();


namespace stm32plus {

  /*
   * Typedefs for easy use
   */

  typedef I2CInterruptFeature<3> I2C3InterruptFeature;


  /**
   * Enabler specialisation, I2C 3
   */

  template<>
  inline void I2CInterruptFeatureEnabler<3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&I2C3_EV_IRQHandler;
    Nvic::configureIrq(I2C3_EV_IRQn,ENABLE,priority,subPriority);
    Nvic::configureIrq(I2C3_ER_IRQn,ENABLE,priority,subPriority);
  }
}
