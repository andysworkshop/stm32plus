/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief A debounced pushbutton with autorepeat
   *
   * This class includes customised delay before the auto-repeat as well as
   * the delay between repeats.
   */

  class AutoRepeatPushButton : public PushButton {

    private:

      /**
       * our internal state
       */

      enum InternalState {

        /// waiting for action
        Idle,

        /// pressed, waiting to start repeating
        WaitingForInitial,

        /// repeating, waiting for next repeat
        WaitingForRepeat
      } _internalState;

      /// the time before repeating starts
      uint32_t _initialDelayMillis;

      /// the time between each repeat
      uint32_t _repeatDelayMillis;

      /// the last time something happened
      uint32_t _lastEventTime;

    public:
      AutoRepeatPushButton(const GpioPinRef& pin,bool pressedState,uint32_t initialDelayMillis,uint32_t repeatDelayMillis);

      ButtonState getState();
  };
}
