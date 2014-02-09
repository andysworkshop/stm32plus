/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * This class implements an automatic fader for PWM channels
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  class PwmFadeTimerDmaFeature : public TimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode> {

    public:
      scoped_array<uint16_t> _compareValues;
      uint16_t _numCompareValues;

    public:
      PwmFadeTimerDmaFeature(Dma& dma);

      void beginFadeByTimer(Timer& timer,const uint8_t *percents,uint16_t numPercents);
      void repeatFadeByTimer(const Timer& timer);
  };


  /*
   * Constructor
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority,uint32_t TDmaMode>
  inline PwmFadeTimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>::PwmFadeTimerDmaFeature(Dma& dma)
    : TimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>(dma) {
  }


  /**
   * Start the fade using the supplied percentage array. The user can let this array go
   * out of scope after this function is called. The fading will start immediately and
   * and not repeat unless DMA circular mode is selected.
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority,uint32_t TDmaMode>
  inline void PwmFadeTimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>::beginFadeByTimer(Timer& timer,const uint8_t *percents,uint16_t numPercents) {

    uint16_t i;
    uint32_t period;

    // get the timer period from the base class

    period=timer.getPeriod()+1;

    // allocate space for the percentages

    _compareValues.reset(new uint16_t[numPercents]);
    _numCompareValues=numPercents;

    // convert each percent to a compare value

    for(i=0;i<numPercents;i++) {

      // watch out for overflow

      if(period<0xFFFFFFFF/100)
        _compareValues[i]=static_cast<uint16_t>((period*static_cast<uint32_t>(percents[i]))/100L);
      else
        _compareValues[i]=(period/100L)*static_cast<uint32_t>(percents[i]);
    }

    // set the DMA off

    this->beginWriteByTimer(timer,_compareValues.get(),numPercents);
  }


  /**
   * Repeat (start again) a timer-based fade. This is only valid for when the DMA is not
   * running in circular (continuous) mode.
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority,uint32_t TDmaMode>
  inline void PwmFadeTimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>::repeatFadeByTimer(const Timer& timer) {
    this->beginWriteByTimer(timer,_compareValues.get(),_numCompareValues);
  }


  /**
   * Types to support easy access to the common use case of using the Update event
   */

  // TIM1

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer1Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer1Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer1Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer1Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer1Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer1Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer1Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer1Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  // TIM2

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer2Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer2Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer2Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer2Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer2Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer2Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer2Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer2Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  // TIM3

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer3Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer3Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer3Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer3Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer3Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer3Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer3Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer3Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

#ifndef STM32PLUS_F0

  // TIM4

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer4Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer4Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer4Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer4Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer4Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer4Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer4Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer4Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  // TIM5

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer5Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer5Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer5Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer5Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer5Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer5Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer5Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer5Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  // TIM7

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer7Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer7Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer7Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer7Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer7Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer7Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer7Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer7Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  // TIM8

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer8Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer8Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer8Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer8Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer8Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer8Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer8Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer8Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

#endif

  // TIM6

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer6Channel1UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer6Ccr1DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer6Channel2UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer6Ccr2DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer6Channel3UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer6Ccr3DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  using Timer6Channel4UpdatePwmFadeTimerDmaFeature = PwmFadeTimerDmaFeature<Timer6Ccr4DmaPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>;
}
