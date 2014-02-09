/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to configure this timer as a master for one or
   * more slave timers.
   * @tparam TOutputSource one of the TIM_TRGOSource values
   */

  template<uint16_t TOutputSource>
  class TimerMasterFeature : public TimerFeatureBase {
    public:
      TimerMasterFeature(Timer& timer);
      void enableMasterFeature() const;
  };

  /**
   * Constructor. Enable master mode and select the output trigger
   */

  template<uint16_t TOutputSource>
  inline TimerMasterFeature<TOutputSource>::TimerMasterFeature(Timer& timer)
    : TimerFeatureBase(timer) {
  }


  /**
   * The master feature cannot be enabled until the rest of the timer is set up
   * so this cannot be done in the constructor
   */

  template<uint16_t TOutputSource>
  inline void TimerMasterFeature<TOutputSource>::enableMasterFeature() const {

    // select the output source and enable master mode

    TIM_SelectMasterSlaveMode(_timer,TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(_timer,TOutputSource);
  }


  /**
   * Typedefs for easy access
   */

  typedef TimerMasterFeature<TIM_TRGOSource_Reset> TimerResetMasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_Enable> TimerEnableMasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_Update> TimerUpdateMasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_OC1> TimerCompareMasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_OC1Ref> TimerOutputCompare1MasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_OC2Ref> TimerOutputCompare2MasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_OC3Ref> TimerOutputCompare3MasterFeature;
  typedef TimerMasterFeature<TIM_TRGOSource_OC4Ref> TimerOutputCompare4MasterFeature;
}
