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

extern "C" void TIM1_UP_TIM10_IRQHandler();
extern "C" void TIM1_BRK_TIM9_IRQHandler();
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler();
extern "C" void TIM1_CC_IRQHandler();
extern "C" void TIM2_IRQHandler();
extern "C" void TIM3_IRQHandler();
extern "C" void TIM4_IRQHandler();
extern "C" void TIM5_IRQHandler();
extern "C" void TIM6_DAC_IRQHandler();
extern "C" void TIM7_IRQHandler();
extern "C" void TIM8_BRK_TIM12_IRQHandler();
extern "C" void TIM8_UP_TIM13_IRQHandler();
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler();
extern "C" void TIM8_CC_IRQHandler();


namespace stm32plus {


  /**
   * Timer feature to handle interrupts
   * @tparam TTimerNumber The number of the timer 1..14
   */

  template<uint8_t TTimerNumber>
  class TimerInterruptFeature : public TimerFeatureBase,
                                public TimerEventSource {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static TimerEventSource *_timerInstance;

    public:
      TimerInterruptFeature(Timer& timer);
      ~TimerInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);

      void clearPendingInterruptsFlag(uint16_t interruptMask) const;
  };

  /*
   * Typedefs for easy use
   */

  typedef TimerInterruptFeature<1> Timer1InterruptFeature;
  typedef TimerInterruptFeature<2> Timer2InterruptFeature;
  typedef TimerInterruptFeature<3> Timer3InterruptFeature;
  typedef TimerInterruptFeature<4> Timer4InterruptFeature;
  typedef TimerInterruptFeature<5> Timer5InterruptFeature;
  typedef TimerInterruptFeature<6> Timer6InterruptFeature;
  typedef TimerInterruptFeature<7> Timer7InterruptFeature;
  typedef TimerInterruptFeature<8> Timer8InterruptFeature;
  typedef TimerInterruptFeature<9> Timer9InterruptFeature;
  typedef TimerInterruptFeature<10> Timer10InterruptFeature;
  typedef TimerInterruptFeature<11> Timer11InterruptFeature;
  typedef TimerInterruptFeature<12> Timer12InterruptFeature;
  typedef TimerInterruptFeature<13> Timer13InterruptFeature;
  typedef TimerInterruptFeature<14> Timer14InterruptFeature;

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
    _nvicPriority=_nvicSubPriority=0;
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
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. TIM_IT_UPDATE, TIM_IT_CC1..4
   */

  template<uint8_t TTimerNumber>
  inline void TimerInterruptFeature<TTimerNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    TimerInterruptFeatureEnabler<TTimerNumber>::enable(interruptMask,_nvicPriority,_nvicSubPriority);
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
  inline void TimerInterruptFeatureEnabler<1>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {

    if((interruptMask & TIM_IT_Update)!=0) {
      _forceLinkage=&TIM1_UP_TIM10_IRQHandler;
      Nvic::configureIrq(TIM1_UP_TIM10_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & TIM_IT_Break)!=0) {
      _forceLinkage=&TIM1_BRK_TIM9_IRQHandler;
      Nvic::configureIrq(TIM1_BRK_TIM9_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & (TIM_IT_COM | TIM_IT_Trigger))!=0) {
      _forceLinkage=&TIM1_TRG_COM_TIM11_IRQHandler;
      Nvic::configureIrq(TIM1_TRG_COM_TIM11_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM1_CC_IRQHandler;
      Nvic::configureIrq(TIM1_CC_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 2
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<2>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM2_IRQHandler;
      Nvic::configureIrq(TIM2_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 3
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<3>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
       _forceLinkage=&TIM3_IRQHandler;
      Nvic::configureIrq(TIM3_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 4
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<4>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM4_IRQHandler;
      Nvic::configureIrq(TIM4_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 5
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<5>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM5_IRQHandler;
      Nvic::configureIrq(TIM5_IRQn,ENABLE,priority,subPriority);
    }
  }

#if defined(STM32PLUS_F4_HAS_TIM6_7_8_12_13_14)

  /**
   * Enabler specialisation, timer 6
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<6>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & TIM_IT_Update)!=0) {
      _forceLinkage=&TIM6_DAC_IRQHandler;
      Nvic::configureIrq(TIM6_DAC_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 7
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<7>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & TIM_IT_Update)!=0) {
      _forceLinkage=&TIM7_IRQHandler;
      Nvic::configureIrq(TIM7_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 8
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<8>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {

    if((interruptMask & TIM_IT_Update)!=0) {
      _forceLinkage=&TIM8_UP_TIM13_IRQHandler;
      Nvic::configureIrq(TIM8_UP_TIM13_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & TIM_IT_Break)!=0) {
      _forceLinkage=&TIM8_BRK_TIM12_IRQHandler;
      Nvic::configureIrq(TIM8_BRK_TIM12_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & (TIM_IT_COM | TIM_IT_Trigger))!=0) {
      _forceLinkage=&TIM8_TRG_COM_TIM14_IRQHandler;
      Nvic::configureIrq(TIM8_TRG_COM_TIM14_IRQn,ENABLE,priority,subPriority);
    }

    if((interruptMask & (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))!=0) {
      _forceLinkage=&TIM8_CC_IRQHandler;
      Nvic::configureIrq(TIM8_CC_IRQn,ENABLE,priority,subPriority);
    }
  }

#endif

  /**
   * Enabler specialisation, timer 9
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<9>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM1_BRK_TIM9_IRQHandler;
      Nvic::configureIrq(TIM1_BRK_TIM9_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 10
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<10>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM1_UP_TIM10_IRQHandler;
      Nvic::configureIrq(TIM1_UP_TIM10_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 11
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<11>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM1_TRG_COM_TIM11_IRQHandler;
      Nvic::configureIrq(TIM1_TRG_COM_TIM11_IRQn,ENABLE,priority,subPriority);
    }
  }

#if defined(STM32PLUS_F4_HAS_TIM6_7_8_12_13_14)

  /**
   * Enabler specialisation, timer 12
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<12>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM8_BRK_TIM12_IRQHandler;
      Nvic::configureIrq(TIM8_BRK_TIM12_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 13
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<13>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM8_UP_TIM13_IRQHandler;
      Nvic::configureIrq(TIM8_UP_TIM13_IRQn,ENABLE,priority,subPriority);
    }
  }


  /**
   * Enabler specialisation, timer 14
   * @param interruptMask TIM_* interrupts to be enabled
   */

  template<>
  inline void TimerInterruptFeatureEnabler<14>::enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority) {
    if((interruptMask & (TIM_IT_Update | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2))!=0) {
      _forceLinkage=&TIM8_TRG_COM_TIM14_IRQHandler;
      Nvic::configureIrq(TIM8_TRG_COM_TIM14_IRQn,ENABLE,priority,subPriority);
    }
  }

#endif

}
