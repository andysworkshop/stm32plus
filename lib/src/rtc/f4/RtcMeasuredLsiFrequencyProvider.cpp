/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/rtc.h"


namespace stm32plus {


  /**
   * Compute the LSI frequency using TIM5 to measure it
   * @return the frequency in Hz
   */

  uint32_t RtcMeasuredLsiFrequencyProvider::getLsiFrequency() {

    /**
     * Use a local class as the observer for the TIM5 channel 4 interrupts
     */

    class T5Observer {

      private:
        volatile bool _completed;
        uint16_t _period;
        uint8_t _captureIndex;
        uint16_t _captures[2];

      public:

        T5Observer() {
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

          if(tet==TimerEventType::EVENT_COMPARE4) {

            if(_captureIndex==0 || _captureIndex==1) {

              _captures[_captureIndex++]=TIM_GetCapture4(TIM5);

              if(_captureIndex==2) {
                _period=(uint16_t)(0xFFFF-_captures[0]+_captures[1]+1);
                _completed=true;
              }
            }
          }
        }

    } interruptObserver;

    // Declare an instance of timer5

    Timer5<
      Timer5InternalClockFeature,         // used only for its calculation of the timer clock frequency
      TimerChannel4Feature<               // we need channel 4
        TimerChannelICRisingEdgeFeature,  // rising edge trigger
        TimerChannelICDirectTiFeature,    // direct TI connection
        TimerChannelICPreScaler8Feature,  // input capture prescaler of 8
        TimerChannelICFilterFeature<0>    // no filter
      >,
      Timer5InterruptFeature,       // we'll be using interrupts
      Timer5RemapLsiFeature         // we will remap LSI output to TIM5 ch4 input
    > timer5;

    // insert our local class as an interrupt event subscriber

    timer5.TimerInterruptEventSender.insertSubscriber(TimerInterruptEventSourceSlot::bind(&interruptObserver,&T5Observer::onTimerEvent));

    // enable interrupts and the timer

    timer5.Timer::enablePeripheral();
    timer5.clearPendingInterruptsFlag(TIM_IT_CC4);
    timer5.enableInterrupts(TIM_IT_CC4);

    // wait until completed

    while(!interruptObserver.completed());

    // disable the timer, interrupts and de-init the timer

    timer5.Timer::disablePeripheral();

    timer5.disableInterrupts(TIM_IT_CC4);
    timer5.deinitialise();

    // return the calculated frequency

    uint32_t frequency=timer5.getClock()/interruptObserver.getPeriod()*8;
    return frequency;
  }
}

#endif
