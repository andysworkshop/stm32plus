/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to support the break functionality included with the advanced and some of the general purpose
   * timers. This feature supports a 'lock' against accidental software changes. The lock mode can only be set
   * once after reset by a write to the BDTR register. Therefore if you want a lock mode other than OFF then you
   * must simulatenously write all the bits you want as well as the desired lock mode in one write access to the
   * BDTR register.
   *
   * To support that, you can use the template parameters to set a lock mode and a default value for the register.
   * Obviously your choice of lock mode will have an impact on whether you can set any other bits in the BDTR
   * register afterwards.
   *
   * @tparam TInitRegValue The initial value for the BDTR register (including the lock mode)
   */

  template<uint16_t TInitRegValue>
  class TimerBreakFeature : public TimerFeatureBase {

    public:

      enum BreakPolarity {
        BREAK_POLARITY_LOW,
        BREAK_POLARITY_HIGH
      };

    public:
      TimerBreakFeature(Timer& timer);

      void enableMainOutput() const;
      void disableMainOutput() const;

      void enableAutomaticOutput() const;
      void disableAutomaticOutput() const;

      void setBreakPolarity(BreakPolarity polarity) const;

      void enableBreak() const;
      void disableBreak() const;

      void enableRunModeOffState() const;
      void disableRunModeOffState() const;

      void enableIdleModeOffState() const;
      void disableIdleModeOffState() const;

      void setDeadTimeGenerator(uint8_t dtg) const;
  };


  /**
   * Provide a typedef for the default case of the BDTR register being unlocked and having
   * a default value of zero.
   */

  typedef TimerBreakFeature<0> UnlockedTimerBreakFeature;


  /**
   * Constructor
   * @param timer reference to the timer object
   */

  template<uint16_t TInitRegValue>
  TimerBreakFeature<TInitRegValue>::TimerBreakFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    ((TIM_TypeDef *)_timer)->BDTR = TInitRegValue;
  }



  /**
   * Master output enable for OC and OCN
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::enableMainOutput() const {
    TIM_CtrlPWMOutputs(_timer,ENABLE);
  }


  /**
   * Disable OC and OCN for this timer
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::disableMainOutput() const {
    TIM_CtrlPWMOutputs(_timer,DISABLE);
  }


  /**
   * The master output enable can be set by software or automatically at the next update event
   * (if the break input is not be active)
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::enableAutomaticOutput() const {
    ((TIM_TypeDef *)_timer)->BDTR |= TIM_AutomaticOutput_Enable;
  }


  /**
   * The master output enable can only be set by software
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::disableAutomaticOutput() const {
    ((TIM_TypeDef *)_timer)->BDTR &= ~TIM_AutomaticOutput_Enable;
  }


  /**
   * Set the polarity of the break input
   * @param polarity BREAK_POLARITY_LOW or BREAK_POLARITY_HIGH
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::setBreakPolarity(BreakPolarity polarity) const {

    if(polarity==BREAK_POLARITY_LOW)
      ((TIM_TypeDef *)_timer)->BDTR &= ~TIM_BreakPolarity_High;
    else
      ((TIM_TypeDef *)_timer)->BDTR |= TIM_BreakPolarity_High;
  }


  /**
   * Break inputs (BRK and CSS clock failure event) enabled
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::enableBreak() const {
    ((TIM_TypeDef *)_timer)->BDTR |= TIM_Break_Enable;
  }


  /**
   * Break inputs (BRK and CSS clock failure event) disabled
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::disableBreak() const {
    ((TIM_TypeDef *)_timer)->BDTR &= ~TIM_Break_Enable;
  }


  /**
   * When inactive, OC/OCN outputs are enabled with their inactive level as soon as CCxE=1
   * or CCxNE=1. Then, OC/OCN enable output signal=1
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::enableRunModeOffState() const {
    ((TIM_TypeDef *)_timer)->BDTR |= TIM_OSSRState_Enable;
  }


  /**
   * When inactive, OC/OCN outputs are disabled (OC/OCN enable output signal=0)
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::disableRunModeOffState() const {
    ((TIM_TypeDef *)_timer)->BDTR &= ~TIM_OSSRState_Enable;
  }


  /**
   * When inactive, OC/OCN outputs are forced first with their idle level as soon as CCxE=1 or
   * CCxNE=1. OC/OCN enable output signal=1)
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::enableIdleModeOffState() const {
    ((TIM_TypeDef *)_timer)->BDTR |= TIM_OSSIState_Enable;
  }


  /**
   * When inactive, OC/OCN outputs are disabled (OC/OCN enable output signal=0)
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::disableIdleModeOffState() const {
    ((TIM_TypeDef *)_timer)->BDTR &= ~TIM_OSSIState_Enable;
  }


  /**
   * Setup the deadtime generator. This value encodes both the resolution of the dead time generator and
   * the value itself. See the documentation in the reference manual for the DTG[7:0] bits in the
   * TIMx_BDTR register for details. LockMode must not be set to any level for this to work.
   * @param dtg The value encoding the resolution and dead time
   */

  template<uint16_t TInitRegValue>
  inline void TimerBreakFeature<TInitRegValue>::setDeadTimeGenerator(uint8_t dtg) const {
    ((TIM_TypeDef *)_timer)->BDTR = (((TIM_TypeDef *)_timer)->BDTR & ~ 0xff) | dtg;
  }
}
