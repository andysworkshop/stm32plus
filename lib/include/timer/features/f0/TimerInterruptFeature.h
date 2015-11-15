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


/*
 * Forward declare the IRQ handler names
 */

extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler();
extern "C" void TIM1_CC_IRQHandler();

#if defined (STM32PLUS_F0_51) || defined(STM32PLUS_F0_42)
extern "C" void TIM2_IRQHandler();
#endif

extern "C" void TIM3_IRQHandler();

#if defined (STM32PLUS_F0_51)
extern "C" void TIM6_DAC_IRQHandler();
#endif

extern "C" void TIM14_IRQHandler();

#if defined (STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)
extern "C" void TIM15_IRQHandler();
#endif

extern "C" void TIM16_IRQHandler();
extern "C" void TIM17_IRQHandler();


namespace stm32plus {


  /**
   * Timer feature to handle interrupts
   * @tparam TTimerNumber The number of the timer 1..17
   */

  template<uint8_t TTimerNumber>
  class TimerInterruptFeature : public TimerFeatureBase,
                                public TimerEventSource {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;

    public:
      static TimerEventSource *_timerInstance;

    public:
      TimerInterruptFeature(Timer& timer);
      ~TimerInterruptFeature();

      void setNvicPriority(uint8_t priority);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);

      void clearPendingInterruptsFlag(uint16_t interruptMask) const;
  };

  /*
   * Typedefs for easy use
   */

  typedef TimerInterruptFeature<1> Timer1InterruptFeature;

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_42)
  typedef TimerInterruptFeature<2> Timer2InterruptFeature;
#endif

  typedef TimerInterruptFeature<3> Timer3InterruptFeature;

#if defined(STM32PLUS_F0_51)
  typedef TimerInterruptFeature<6> Timer6InterruptFeature;
#endif

  typedef TimerInterruptFeature<14> Timer14InterruptFeature;

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)
  typedef TimerInterruptFeature<15> Timer15InterruptFeature;
#endif

  typedef TimerInterruptFeature<16> Timer16InterruptFeature;
  typedef TimerInterruptFeature<17> Timer17InterruptFeature;


  /**
   * Template static data member initialisation
   */

  template<uint8_t TTimerNumber>
  TimerEventSource *TimerInterruptFeature<TTimerNumber>::_timerInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TTimerNumber>
  inline TimerInterruptFeature<TTimerNumber>::TimerInterruptFeature(Timer& timer)
    : TimerFeatureBase(timer) {
    _interruptMask=0;
    _nvicPriority=0;
    _timerInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TTimerNumber>
  inline TimerInterruptFeature<TTimerNumber>::~TimerInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority number.  This function just remembers your
   * number. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::setNvicPriority(uint8_t priority) {
    _nvicPriority=priority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. TIM_IT_UPDATE, TIM_IT_CC1..4
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    TimerInterruptFeatureEnabler<TTimerNumber>::enable(interruptMask,_nvicPriority);
    TIM_ITConfig(_timer,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. TIM_IT_UPDATE, TIM_IT_CC1..4
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    TIM_ITConfig(_timer,interruptMask,DISABLE);
  }


  /**
   * Clear the selected pending interrupt flags
   * @param interruptMask The bitmask of interrupts, e.g. TIM_IT_UPDATE, TIM_IT_CC1..4
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::clearPendingInterruptsFlag(uint16_t interruptMask) const {
    TIM_ClearITPendingBit(_timer,interruptMask);
  }


  /**
   * Enabler specialisation, timer 1
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<1>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {

    if((interruptMask & (TIM_IT_COM | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_Update))!=0) {
      _forceLinkage=&TIM1_BRK_UP_TRG_COM_IRQHandler;
      Nvic::configureIrq(TIM1_BRK_UP_TRG_COM_IRQn,ENABLE,priority);
    }

    if((interruptMask & (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM1_CC_IRQHandler;
      Nvic::configureIrq(TIM1_CC_IRQn,ENABLE,priority);
    }
  }

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_42)

  /**
   * Enabler specialisation, timer 2
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<2>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM2_IRQHandler;
      Nvic::configureIrq(TIM2_IRQn,ENABLE,priority);
    }
  }

#endif

  /**
   * Enabler specialisation, timer 3
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<3>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
       _forceLinkage=&TIM3_IRQHandler;
      Nvic::configureIrq(TIM3_IRQn,ENABLE,priority);
    }
  }

#if defined(STM32PLUS_F0_51)
  /**
   * Enabler specialisation, timer 6
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<6>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & TIM_IT_Update)!=0) {
      _forceLinkage=&TIM6_DAC_IRQHandler;
      Nvic::configureIrq(TIM6_DAC_IRQn,ENABLE,priority);
    }
  }
#endif

  /**
   * Enabler specialisation, timer 14
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<14>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM14_IRQHandler;
      Nvic::configureIrq(TIM14_IRQn,ENABLE,priority);
    }
  }

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)

  /**
   * Enabler specialisation, timer 15
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<15>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM15_IRQHandler;
      Nvic::configureIrq(TIM15_IRQn,ENABLE,priority);
    }
  }
#endif

  /**
   * Enabler specialisation, timer 16
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<16>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM16_IRQHandler;
      Nvic::configureIrq(TIM16_IRQn,ENABLE,priority);
    }
  }


  /**
   * Enabler specialisation, timer 17
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<17>::enable(uint16_t interruptMask,uint8_t priority,uint8_t /* subPriority */) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM17_IRQHandler;
      Nvic::configureIrq(TIM17_IRQn,ENABLE,priority);
    }
  }
}
