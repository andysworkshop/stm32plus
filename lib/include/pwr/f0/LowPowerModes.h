/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


extern "C" void SystemInit();


namespace stm32plus {

  /**
   * @brief Allow the STM32 to be put into one of the low power modes.
   *
   * Convenience class to wrap the functions in the std peripheral library.
   */

  class LowPowerModes {

    protected:
      uint32_t _wakeUpPin;

    protected:
      void doStop(uint32_t regulatorMode,uint8_t entry);

    public:
      LowPowerModes(uint32_t wakeUpPin=PWR_WakeUpPin_1);

      void standby();

      void stopInterruptWakeup(uint32_t regulatorMode=PWR_Regulator_LowPower);
      void stopEventWakeup(uint32_t regulatorMode=PWR_Regulator_LowPower);

      void enableWakeup() const;
      void disableWakeup() const;

      void disable() const;

      static bool wasWokenUp();
      static bool wasInStandby();
  };


  /**
   * Constructor. Enable access to the backup domain and enable the wakeup pin
   */

  inline LowPowerModes::LowPowerModes(uint32_t wakeUpPin)
    : _wakeUpPin(wakeUpPin) {

    // power and backup domain clocks on

    ClockControl<PERIPHERAL_POWER>::On();

    // wakeup pin enable, backup access enable

    PWR_BackupAccessCmd( ENABLE);
    PWR_WakeUpPinCmd(_wakeUpPin,ENABLE);
  }


  /**
   * disable pins and stop clocks
   */

  inline void LowPowerModes::disable() const {

    // clean up

    PWR_WakeUpPinCmd(_wakeUpPin,DISABLE);
    PWR_BackupAccessCmd( DISABLE);

    ClockControl<PERIPHERAL_POWER>::Off();
  }


  /*
   * Set the processor to STOP mode
   */

  inline void LowPowerModes::doStop(uint32_t regulatorMode,uint8_t entry) {

    bool systickInterruptEnabled;

    // Systick interrupt cannot be enabled during stop mode
    // See: STM32F103xC/D/E errata: "Debugging Stop mode and system tick timer"

    systickInterruptEnabled=(SysTick->CTRL & SysTick_CTRL_TICKINT_Msk) != 0;
    if(systickInterruptEnabled)
      SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;

    // enter stop mode

    PWR_EnterSTOPMode(regulatorMode,entry);

    // re-enable clocks - you must supply this function in your app, and you must have done in order
    // to run anything at all though you may have called it something different.

    ::SystemInit();

    // re-enable Systick if it was previously on. If a Systick would have fired during the stop mode
    // then that tick is lost, i.e. the interrupt pending bit is not set on wakeup.

    if(systickInterruptEnabled)
      SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
  }


  /**
   * Enter standby mode
   */

  inline void LowPowerModes::standby() {
    PWR_EnterSTANDBYMode();
  }


  /**
   * Enter stop mode, waking on EXTI interrupt.
   * @param[in] regulatorMode PWR_Regulator_ON or PWR_Regulator_LowPower determines if the voltage regulator
   *  goes into low power or not
   */

  inline void LowPowerModes::stopInterruptWakeup(uint32_t regulatorMode) {
    doStop(regulatorMode,PWR_STOPEntry_WFI);
  }


  /**
   * Enter stop mode, waking on EXTI event.
   * @param[in] regulatorMode PWR_Regulator_ON or PWR_Regulator_LowPower determines if the voltage regulator
   *  goes into low power or not
   */

  inline void LowPowerModes::stopEventWakeup(uint32_t regulatorMode) {
    doStop(regulatorMode,PWR_STOPEntry_WFE);
  }


  /**
   * Return true if we're running after a wakeup event
   * @return true if wakeup has happened
   */

  inline bool LowPowerModes::wasWokenUp() {
    return PWR_GetFlagStatus(PWR_FLAG_WU)!=RESET;
  }


  /**
   * Return true if we were in standby mode before this run
   * @return true if we were previously in standby
   */

  inline bool LowPowerModes::wasInStandby() {
    return PWR_GetFlagStatus(PWR_FLAG_SB)!=RESET;
  }


  /**
   * Enable the WAKEUP pin
   */

  inline void LowPowerModes::enableWakeup() const {
    PWR_WakeUpPinCmd(_wakeUpPin,ENABLE);
  }


  /**
   * Disable the WAKEUP pin
   */

  inline void LowPowerModes::disableWakeup() const {
    PWR_WakeUpPinCmd(_wakeUpPin,DISABLE);
  }
}
