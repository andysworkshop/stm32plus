/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief EXTI base class. This holds the instance address of the IRQ handler class and
   * handles the peripheral initialisation (non-template code)
   */

  DECLARE_EVENT_SIGNATURE(ExtiInterrupt,void(uint8_t));

  class ExtiPeripheralBase {

    protected:
      uint32_t _line;

    protected:
      void initialisePeripheral(uint32_t line,EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger);

    public:
      DECLARE_EVENT_SOURCE(ExtiInterrupt);

    public:
      void clearPendingInterrupt() const;
  };


  /**
   * Initialise the peripheral
   *
   * @param[in] index exti number (0..n)
   * @param[in] mode EXTI_Mode_Event or EXTI_Mode_Interrupt
   * @param[in] trigger EXTI_Trigger_Falling / EXTI_Trigger_Rising / EXTI_Trigger_Rising_Falling
   * @param[in] pin The GPIO pin to use.
   */

  inline void ExtiPeripheralBase::initialisePeripheral(uint32_t line,EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger) {

    EXTI_InitTypeDef init;

    _line=line;

    init.EXTI_Line=line;
    init.EXTI_Mode=mode;
    init.EXTI_Trigger=trigger;
    init.EXTI_LineCmd=ENABLE;

    EXTI_Init(&init);
  }


  /**
   * Clear any pending interrupt flag
   */

  inline void ExtiPeripheralBase::clearPendingInterrupt() const {
    EXTI_ClearITPendingBit(_line);
  }
}
