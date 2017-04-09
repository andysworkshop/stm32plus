/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F4)
#error This include file is only for the F4
#endif


namespace stm32plus {

  /**
   * @brief F4 implementation of the NVIC
   *
   * The NVIC methods allow us to configure the number of bits available for priority and subpriority. The default
   * is all 4 bits for priority and none for subpriority. Other methods allow us to enable/disable the IRQ for
   * a given peripheral.
   */

  class Nvic {

    public:
      static void initialise(uint32_t priorityGroup=NVIC_PriorityGroup_4);
      static void configureIrq(uint8_t interrupt,FunctionalState state=ENABLE,uint8_t preemptionPriority=0,uint8_t subPriority=0);
      static void disableAllInterrupts();
      static void enableAllInterrupts();
      static bool isAnyIrqActive();
      static void systemReset();
  };


  /**
   * Initialise the Nvic group that determines the bits available for priority/subpriority
   */

  inline void Nvic::initialise(uint32_t priorityGroup) {
    NVIC_PriorityGroupConfig(priorityGroup);
  }


  /**
   * Enable or disable an interrupt.
   * @param[in] interrupt The peripheral identifier.
   * @param[in] state The new state, ENABLE or DISABLE. Default is ENABLE.
   * @param[in] preemptionPriority The new preemption priority. Default is 0.
   * @param[in] subPriority The new preemption subpriority. Default is 0.
   */

  inline void Nvic::configureIrq(uint8_t interrupt,FunctionalState state,uint8_t preemptionPriority,uint8_t subPriority) {

    NVIC_InitTypeDef nit;

    nit.NVIC_IRQChannel=interrupt;
    nit.NVIC_IRQChannelPreemptionPriority=preemptionPriority;
    nit.NVIC_IRQChannelSubPriority=subPriority;
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
   * Return true if any IRQ is currently active
   */

  inline bool Nvic::isAnyIrqActive() {
    return NVIC->IABR[0]!=0 ||
           NVIC->IABR[1]!=0 ||
           NVIC->IABR[2]!=0 ||
           NVIC->IABR[3]!=0 ||
           NVIC->IABR[4]!=0 ||
           NVIC->IABR[5]!=0 ||
           NVIC->IABR[6]!=0 ||
           NVIC->IABR[7]!=0;
  }
  
  
  /**
   * Trigger a soft reset of the MCU.
   */
  
  inline void Nvic::systemReset() {
    NVIC_SystemReset(); 
  }
}
