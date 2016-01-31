/*
 * Andy's Workshop Reflow Oven Controller.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace stm32plus {


  /**
   * The input whose edge will trigger a count
   */

  enum class EncoderCounterEdge {
    Input1,           ///! TI1 depending on the level of TI2
    Input2,           ///! TI2 depending on the level of TI1
    Inputs1And2       ///! TI1 and TI2 depending on the other level
  };


  /**
   * Encoder edge polarity, i.e. which edge transition triggers a count
   */

  enum class EncoderPolarity {
    Rising,           ///! Rising edge transition
    Falling           ///! Falling edge transition
  };


  /**
   * Provide support for the timer encoder feature. If the optional Exti instance is provided
   * then this feature will subscribe to its interrupt and reset the counter to a known value
   * when the interrupt is triggered.
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  class TimerEncoderFeature : public TimerFeatureBase {

    protected:
      ExtiPeripheralBase *_managedReset;
      uint32_t _resetValue;

    protected:
      void onExtiInterrupt(uint8_t );

    public:
      TimerEncoderFeature(Timer& timer);
      ~TimerEncoderFeature();

      void initialiseEncoderCounter(uint32_t period);
      void manageEncoderReset(ExtiPeripheralBase& exti,uint32_t resetValue);
  };


  /**
   * Constructor
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  inline TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::TimerEncoderFeature(Timer& timer)
    : TimerFeatureBase(timer),
      _managedReset(nullptr) {

    // static checks

    static_assert(TEdge==EncoderCounterEdge::Input1 || TEdge==EncoderCounterEdge::Input2 || TEdge==EncoderCounterEdge::Inputs1And2,"Invalid counter edge");
    static_assert(TInput1Polarity==EncoderPolarity::Rising || TInput1Polarity==EncoderPolarity::Falling,"Invalid polarity for input 1");
    static_assert(TInput2Polarity==EncoderPolarity::Rising || TInput2Polarity==EncoderPolarity::Falling,"Invalid polarity for input 2");

    // set the encoder mode based on the constant parameters

    TIM_EncoderInterfaceConfig(
        _timer,
        TEdge==EncoderCounterEdge::Input1 ? TIM_EncoderMode_TI1 : TEdge==EncoderCounterEdge::Input2 ? TIM_EncoderMode_TI2 : TIM_EncoderMode_TI12,
        TInput1Polarity==EncoderPolarity::Rising ? TIM_ICPolarity_Rising : TIM_ICPolarity_Falling,
        TInput2Polarity==EncoderPolarity::Rising ? TIM_ICPolarity_Rising : TIM_ICPolarity_Falling);
  }


  /**
   * Destructor - unsubscribe to the reset signal if there is one
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  inline TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::~TimerEncoderFeature() {

    if(_managedReset) {
      _managedReset->ExtiInterruptEventSender.removeSubscriber(
        ExtiInterruptEventSourceSlot::bind(this,&TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::onExtiInterrupt)
      );
    }
  }


  /**
   * Simple counter initialiser specifically for the encoder feature
   * @param period The up-limit (exclusive)
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  inline void TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::initialiseEncoderCounter(uint32_t period) {
    _timer.initialiseTimeBase(period,0,TIM_CKD_DIV1,TIM_CounterMode_Up);
  }


  /**
   * Manage the automatic encoder reset (the encoder index pin). You must have set up the Exti instance
   * to trigger on the correct edge and pin according to your setup. Don't let the Exti instance go
   * out of scope before this feature class.
   * @param exti The Exti instance that will raise an interrupt when the encoder is to be reset
   * @param resetValue The value to reset the counter to when the interrupt happens.
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  inline void TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::manageEncoderReset(ExtiPeripheralBase& exti,uint32_t resetValue) {

    // store the parameters

    _managedReset=&exti;
    _resetValue=resetValue;

    // subscribe to exti interrupts

    exti.ExtiInterruptEventSender.insertSubscriber(
      ExtiInterruptEventSourceSlot::bind(this,&TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::onExtiInterrupt)
    );
  }


  /**
   * Interrupt callback from the EXTI interrupt
   */

  template<EncoderCounterEdge TEdge,EncoderPolarity TInput1Polarity,EncoderPolarity TInput2Polarity>
  inline void TimerEncoderFeature<TEdge,TInput1Polarity,TInput2Polarity>::onExtiInterrupt(uint8_t /* extiLine */) {
    _timer.setCounter(_resetValue);
  }
}
