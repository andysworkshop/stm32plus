/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F0)
#error This include file is only for the F0
#endif


namespace stm32plus {

  /**
   * @brief F0 implementation of the NVIC
   *
   * The F0 NVIC is simpler than the F1/F4. Channel priorities are supported in a range of 0..3 where
   * zero is the highest but there are no priority groups or sub-priorities. Method interface
   * compatibility is maintained between this and the F1/F4 implementation.
   */

  class Nvic {

    public:
      static void initialise(uint32_t priorityGroup=0);
      static void configureIrq(uint8_t interrupt,FunctionalState state=ENABLE,uint8_t preemptionPriority=0,uint8_t subPriority=0);
      static void disableAllInterrupts();
      static void enableAllInterrupts();
      static void systemReset();
  };


  /**
   * Initialise the Nvic group that determines the bits available for priority/subpriority
   */

  inline void Nvic::initialise(uint32_t /* priorityGroup */) {
    // nothing to do
  }


  /**
   * Enable or disable an interrupt.
   * @param[in] interrupt The peripheral identifier.
   * @param[in] state The new state, ENABLE or DISABLE. Default is ENABLE.
   * @param[in] preemptionPriority The new preemption priority. Default is 0.
   * @param[in] subPriority The new preemption subpriority. Default is 0.
   */

  inline void Nvic::configureIrq(uint8_t interrupt,FunctionalState state,uint8_t preemptionPriority,uint8_t /* subPriority */) {

    NVIC_InitTypeDef nit;

    nit.NVIC_IRQChannel=interrupt;
    nit.NVIC_IRQChannelPriority=preemptionPriority;
    nit.NVIC_IRQChannelCmd=state;

    NVIC_Init(&nit);
  }


  /**
   * Disable all the interrupts
   */

  inline void Nvic::disableAllInterrupts() {
    __disable_irq();
  }


  /**
   * Enable all the interrupts
   */

  inline void Nvic::enableAllInterrupts() {
    __enable_irq();
  }


  /**
   * Trigger a soft reset of the MCU.
   */

  inline void Nvic::systemReset() {
    NVIC_SystemReset();
  }
}
