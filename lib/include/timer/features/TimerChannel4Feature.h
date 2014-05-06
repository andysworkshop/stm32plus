/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to support the operations available on
   * a specific timer channel
   */

  template<>
  class TimerChannelFeature<4> : public TimerChannelFeatureBase {

    protected:
      uint8_t _dutyCycle;

    public:
      TimerChannelFeature(Timer& timer);

      uint32_t getCapture() const;
      void initCompareForPwmOutput(uint8_t initialDutyCycle=0,uint16_t ocMode=TIM_OCMode_PWM1,uint16_t ocPolarity=TIM_OCPolarity_High);
      void initCompare(uint32_t compareValue,uint16_t ocMode=TIM_OCMode_Toggle,uint16_t polarity=TIM_OCPolarity_Low,uint16_t preload=TIM_OCPreload_Disable);
      void initCapture(uint16_t polarity=TIM_ICPolarity_Rising,uint16_t selection=TIM_ICSelection_DirectTI,uint16_t prescaler=TIM_ICPSC_DIV1,uint16_t filter=0,uint16_t timerPrescaler=1);
      void setCompare(uint32_t compare) const;
      void setDutyCycle(uint8_t dutyCycle);
  };


  /*
   * Typedef for easier access
   */

  typedef TimerChannelFeature<4> TimerChannel4Feature;


  /**
   * Constructor
   * @param timer
   */

  inline TimerChannelFeature<4>::TimerChannelFeature(Timer& timer)
    : TimerChannelFeatureBase(timer) {
  }


  /**
   * Get the capture value for this channel
   */

  inline uint32_t TimerChannelFeature<4>::getCapture() const {
    return TIM_GetCapture4(_timer);
  }


  /**
   * Set the compare value for this channel
   */

  inline void TimerChannelFeature<4>::setCompare(uint32_t compareValue) const {
    return TIM_SetCompare4(_timer,compareValue);
  }


  /**
   * Initialise the output compare mode and first value.
   * @param compareValue the timer tick value to compare to
   * @param ocMode What to do when matched (default is TIM_OCMode_Toggle)
   * @param polarity value. Default is TIM_OCPolarity_Low
   * @param preload The preload enable/disable flag. Default is TIM_OCPreload_Disable.
   */

  inline void TimerChannelFeature<4>::initCompare(uint32_t compareValue,uint16_t ocMode,uint16_t polarity,uint16_t preload) {

    TIM_OCInitTypeDef oci;

    // initialise the channel OC

    TIM_OCStructInit(&oci);

    oci.TIM_OCMode=ocMode;
    oci.TIM_OutputState=TIM_OutputState_Enable;
    oci.TIM_Pulse=compareValue;
    oci.TIM_OCPolarity=polarity;

    TIM_OC4Init(_timer,&oci);
    TIM_OC4PreloadConfig(_timer,preload);
    TIM_CtrlPWMOutputs(_timer,ENABLE);
  }


  /**
   * Initialise the channel in PWM output mode.
   * @param initialDutyCycle Default is zero. Duty cycle is a percentage, 0..100
   * @param ocMode Default is TIM_OCMode_PWM1 (edge aligned). Use TIM_OCMode_PWM2 for center aligned.
   * @param ocPolarity Default is TIM_OCPolarity_High
   */

  inline void TimerChannelFeature<4>::initCompareForPwmOutput(uint8_t initialDutyCycle,uint16_t ocMode,uint16_t ocPolarity) {
    initCompare(0,ocMode,ocPolarity);
    setDutyCycle(initialDutyCycle);
  }


  /**
   * Set the duty cycle for this PWM feature
   * @param dutyCycle The duty cycle as a percentage (0..100)
   */

  inline void TimerChannelFeature<4>::setDutyCycle(uint8_t dutyCycle) {

    uint32_t compareValue,period;

    // remember the setting

    _dutyCycle=dutyCycle;

    // get the timer period from the base class

    period=_timer.getPeriod()+1;

    // watch out for overflow

    if(period<0xFFFFFFFF/100)
      compareValue=(period*static_cast<uint32_t>(dutyCycle))/100L;
    else
      compareValue=(period/100L)*static_cast<uint32_t>(dutyCycle);

    setCompare(compareValue);
  }


  /**
   *  Initialise the channel in input capture mode
   *  @param polarity default is TIM_ICPolarity_Rising
   *  @param selection default is TIM_ICSelection_DirectTI
   *  @param prescaler default is TIM_ICPSC_DIV1 (= number of captures to skip)
   *  @param filter default is zero
   *  @param timerPrescaler prescaler for the timer. Default is 1.
   */

  inline void TimerChannelFeature<4>::initCapture(uint16_t polarity,uint16_t selection,uint16_t prescaler,uint16_t filter,uint16_t timerPrescaler) {

    TIM_ICInitTypeDef init;

    // do the peripheral initialisation

    init.TIM_Channel=TIM_Channel_4;
    init.TIM_ICPolarity=polarity;
    init.TIM_ICSelection=selection;
    init.TIM_ICPrescaler=prescaler;
    init.TIM_ICFilter=filter;

    TIM_ICInit(_timer,&init);
    _timer.setPrescaler(timerPrescaler,TIM_PSCReloadMode_Immediate);
  }
}
