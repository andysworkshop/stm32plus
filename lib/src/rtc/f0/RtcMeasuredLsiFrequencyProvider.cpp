/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F0)

#include "config/rtc.h"


namespace stm32plus {


  /**
   * Compute the LSI frequency using TIM14 to measure it
   * @return the frequency in Hz
   */

  uint32_t RtcMeasuredLsiFrequencyProvider::getLsiFrequency() {

    /**
     * Use a local class as the observer for the TIM14 channel 1 interrupts
     */

    class T14Observer {

      private:
        volatile bool _completed;
        uint16_t _period;
        uint8_t _captureIndex;
        uint16_t _captures[2];

      public:

        T14Observer() {
          _completed=false;
          _captureIndex=0;
        }

        bool completed() const {
          return _completed;
        }

        uint16_t getPeriod() const {
          return _period;
        }

        void onTimerEvent(TimerEventType tet,uint8_t /* timerNumber */) {

          if(tet==TimerEventType::EVENT_COMPARE1) {

            if(_captureIndex==0 || _captureIndex==1) {

              _captures[_captureIndex++]=TIM_GetCapture1(TIM14);

              if(_captureIndex==2) {
                _period=(uint16_t)(0xFFFF-_captures[0]+_captures[1]+1);
                _completed=true;
              }
            }
          }
        }

    } interruptObserver;

    // Declare an instance of timer14

    Timer14<
      Timer14InternalClockFeature,        // used only for its calculation of the timer clock frequency
      TimerChannel1Feature<               // we need channel 1
        TimerChannelICRisingEdgeFeature,  // rising edge trigger
        TimerChannelICDirectTiFeature,    // direct TI connection
        TimerChannelICPreScaler8Feature,  // input capture prescaler of 8
        TimerChannelICFilterFeature<0>    // no filter
      >,
      Timer14InterruptFeature,        // we'll be using interrupts
      Timer14RemapRtcClkFeature       // we will remap RTC output to TIM14 ch1 input
    > timer14;

    // insert our local class as an interrupt event subscriber

    timer14.TimerInterruptEventSender.insertSubscriber(TimerInterruptEventSourceSlot::bind(&interruptObserver,&T14Observer::onTimerEvent));

    // enable interrupts and the timer

    timer14.Timer::enablePeripheral();
    timer14.clearPendingInterruptsFlag(TIM_IT_CC1);
    timer14.enableInterrupts(TIM_IT_CC1);

    // wait until completed

    while(!interruptObserver.completed());

    // disable the timer, interrupts and de-init the timer

    timer14.Timer::disablePeripheral();

    timer14.disableInterrupts(TIM_IT_CC1);
    timer14.deinitialise();

    // return the calculated frequency

    uint32_t frequency=timer14.getClock()/interruptObserver.getPeriod()*8;
    return frequency;
  }
}

#endif
