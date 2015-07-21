/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Base class for character based LCD displays
     * @tparam TImpl The subclass providing the implementation
     */

    template<class TImpl>
    class CharacterLcd {

      public:

        /**
         * Possible display states
         */

        enum DisplayState {
          /// The display is currently on
          DISPLAY_ON,
          /// The display is currently off
          DISPLAY_OFF
        };

        /**
         * Possible cursor blink states
         */

        enum BlinkState {
          /// blinking is on
          BLINK_ON,
          /// blinking is off
          BLINK_OFF
        };

        /**
         * Possible cursor visibility states
         */

        enum CursorState {
          /// cursor is on
          CURSOR_ON,
          /// cursor is off
          CURSOR_OFF
        };

        /**
         * Possible scroll directions
         */

        enum ScrollDirection {
          /// scroll to the left
          SCROLL_LEFT,
          /// scroll to the right
          SCROLL_RIGHT
        };

        /**
         * Possible text output direction
         */

        enum TextDirection {
          /// left to right
          TEXT_DIRECTION_LR,
          /// right to left
          TEXT_DIRECTION_RL
        };

        /**
         * Possible auto-scroll states
         */

        enum AutoScrollState {
          /// auto scrolling is on
          AUTO_SCROLL_ON,
          /// auto scrolling is off
          AUTO_SCROLL_OFF
        };

      public:
        void writeString(const char *str);
    };


    /**
     * Write a string of characters to the display. This convenience goes into a loop
     * and calls write().
     *
     * @param str The null terminated string to write.
     */

    template<class TImpl>
    void CharacterLcd<TImpl>::writeString(const char *str) {
      while(*str)
        static_cast<TImpl *>(this)->write(*str++);
    }
  }
}
