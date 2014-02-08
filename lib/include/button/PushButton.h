/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

/**
 * @brief Pushbutton with debouncing
 */

  class PushButton {

    private:

      /// configuration
      static const uint32_t DEBOUNCE_DELAY_MILLIS=50;

      /// Internal button state
      enum InternalState {
        Idle,                         // nothing happening
        DebounceDelay                 // delaying...
      };

      /// The digital pin where the button is connected
      uint8_t _digitalPin;

      /// The pressed state (true=HIGH/falas=LOW)
      bool _pressedState;

      /// Internal state of the class
      InternalState _internalState;

      /// The last time we sampled our button
      uint32_t _lastTime;

      /// The pin
      GpioPinRef _pin;

    public:

      /**
       * Possible button states
       */

      enum ButtonState {
        /// button is up
        NotPressed,

        /// button is down
        Pressed
      };

      PushButton(const GpioPinRef& pin,bool pressedState);
      ButtonState getState();
  };
}
