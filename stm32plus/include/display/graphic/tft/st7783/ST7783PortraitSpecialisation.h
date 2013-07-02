/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/st7783/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of ST7783Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class ST7783Orientation<PORTRAIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        ST7783Orientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationEntryMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;
        void moveTo(const Rectangle& rc) const;
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline ST7783Orientation<PORTRAIT,TAccessMode>::ST7783Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for portrait
     */

    template<class TAccessMode>
    constexpr inline uint16_t ST7783Orientation<PORTRAIT,TAccessMode>::getOrientationEntryMode() const {
      using namespace st7783;
      return ID0 | ID1;
    }


    /**
     * Get the width in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t ST7783Orientation<PORTRAIT,TAccessMode>::getWidth() const {
      return 240;
    }


    /**
     * Get the height in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t ST7783Orientation<PORTRAIT,TAccessMode>::getHeight() const {
      return 320;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void ST7783Orientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_START,rc.X);
      this->_accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_END,rc.X+rc.Width-1);

      this->_accessMode.writeCommand(st7783::VERTICAL_ADDRESS_START,rc.Y);
      this->_accessMode.writeCommand(st7783::VERTICAL_ADDRESS_END,rc.Y+rc.Height-1);

      this->_accessMode.writeCommand(st7783::RAM_HORIZONTAL_ADDRESS,rc.X);
      this->_accessMode.writeCommand(st7783::RAM_VERTICAL_ADDRESS,rc.Y);
    }
  }
}

