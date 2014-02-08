/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/button.h"


namespace stm32plus {

  /**
   * Constructor
   * @param[in] pin The pin that the button is wired to.
   * @param[in] pressedState true if the pressed state is HIGH, false if it is low.
   * @param[in] initialDelayMillis The delay before repeating starts.
   * @param[in] repeatDelayMillis The delay between repeats.
   */

  AutoRepeatPushButton::AutoRepeatPushButton(const GpioPinRef& pin,bool pressedState,uint32_t initialDelayMillis,uint32_t repeatDelayMillis)
  : PushButton(pin,pressedState) {

    // setup the encapsulated class and remember params

    _initialDelayMillis=initialDelayMillis;
    _repeatDelayMillis=repeatDelayMillis;
  }


  /**
   * Get the current state of the button.
   * @return The current state.
   */

  PushButton::ButtonState AutoRepeatPushButton::getState() {

    uint32_t now;

    // if button is not pressed then our state machine is reset

    if(PushButton::getState()==NotPressed) {
      _internalState=Idle;
      return NotPressed;
    }

    now=MillisecondTimer::millis();

    switch(_internalState) {

      // first press, record that we are now waiting for the
      // initial repeat and save the time we started.

      case Idle:
        _internalState=WaitingForInitial;
        _lastEventTime=now;
        return Pressed;

        // lead up to the initial repeat. When the time is reached
        // advance the state into the multi-repeat stage

      case WaitingForInitial:
        if(now-_lastEventTime>=_initialDelayMillis)
        {
          _internalState=WaitingForRepeat;
          _lastEventTime=now;
          return Pressed;
        }
        else
          return NotPressed;

        // we're in the repeat loop. Return 'Pressed' each time
        // the time interval is passed

      case WaitingForRepeat:
        if(now-_lastEventTime>=_repeatDelayMillis)
        {
          _lastEventTime=now;
          return Pressed;
        }
        else
          return NotPressed;
    }

    // should never get here

    return NotPressed;
  }
}
