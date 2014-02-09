/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to configure this timer as a slave for another timer. The
   * slave mode and input trigger (0..3) are configurable.
   * @tparam TInputTrigger one of TIM_InputTriggerSource
   * @tparam TSlaveMode one of TIM_SlaveMode
   */

  template<uint16_t TInputTrigger,uint16_t TSlaveMode>
  class TimerSlaveFeature : public TimerFeatureBase {
    public:
      TimerSlaveFeature(Timer& timer);
      void enableSlaveFeature() const;
  };

  /**
   * Constructor. Enable slave mode and select the input trigger
   */

  template<uint16_t TInputTrigger,uint16_t TSlaveMode>
  inline TimerSlaveFeature<TInputTrigger,TSlaveMode>::TimerSlaveFeature(Timer& timer)
    : TimerFeatureBase(timer) {
  }


  /**
   * Master/slave configuration must come after the rest of the timer has been
   * set up, including channel output compare so this cannot be done in the
   * constructor
   */

  template<uint16_t TInputTrigger,uint16_t TSlaveMode>
  inline void TimerSlaveFeature<TInputTrigger,TSlaveMode>::enableSlaveFeature() const {

    // select the input source and enable slave mode

    TIM_SelectSlaveMode(_timer,TSlaveMode);
    TIM_SelectInputTrigger(_timer,TInputTrigger);
  }


  /**
   * types for the four numbered input triggers
   * See Tables 80,84, in RM0008 for the ITRx to master/slave mappings
   *
   * Table 84 configurations:
   */

  template<uint16_t TSlaveMode> using Timer1MasterTimer2SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer1MasterTimer3SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer1MasterTimer4SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer2MasterTimer5SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer8MasterTimer2SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer2MasterTimer3SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer2MasterTimer4SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer3MasterTimer5SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer3MasterTimer2SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer5MasterTimer3SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer3MasterTimer4SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer4MasterTimer5SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer4MasterTimer2SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer4MasterTimer3SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer8MasterTimer4SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer8MasterTimer5SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;

  /**
   * RM0008 Table 80 configurations.
   */

  template<uint16_t TSlaveMode> using Timer5MasterTimer1SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer1MasterTimer8SlaveFeature = TimerSlaveFeature<TIM_TS_ITR0,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer2MasterTimer1SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer2MasterTimer8SlaveFeature = TimerSlaveFeature<TIM_TS_ITR1,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer3MasterTimer1SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer4MasterTimer8SlaveFeature = TimerSlaveFeature<TIM_TS_ITR2,TSlaveMode>;

  template<uint16_t TSlaveMode> using Timer4MasterTimer1SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;
  template<uint16_t TSlaveMode> using Timer5MasterTimer8SlaveFeature = TimerSlaveFeature<TIM_TS_ITR3,TSlaveMode>;
}
