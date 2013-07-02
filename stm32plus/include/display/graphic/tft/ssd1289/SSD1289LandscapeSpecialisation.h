/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ssd1289/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of SSD1289Orientation for the panel in landscape mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class SSD1289Orientation<LANDSCAPE,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        SSD1289Orientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationEntryMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;
        void moveTo(const Rectangle& rc) const;

        void setScrollPosition(int16_t scrollPosition);
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline SSD1289Orientation<LANDSCAPE,TAccessMode>::SSD1289Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for landscape
     */

    template<class TAccessMode>
    constexpr inline uint16_t SSD1289Orientation<LANDSCAPE,TAccessMode>::getOrientationEntryMode() const {
      return 0x18;      // AM=1, ID[1..0]=01
    }


    /**
     * Get the width in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t SSD1289Orientation<LANDSCAPE,TAccessMode>::getWidth() const {
      return 320;
    }


    /**
     * Get the height in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t SSD1289Orientation<LANDSCAPE,TAccessMode>::getHeight() const {
      return 240;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<LANDSCAPE,TAccessMode>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(ssd1289::HORIZONTAL_POSITION,((rc.Y+rc.Height-1) << 8) | rc.Y);
      this->_accessMode.writeCommand(ssd1289::SET_GDDRAM_X,rc.Y);

      this->_accessMode.writeCommand(ssd1289::VERTICAL_POSITION_START,rc.X);
      this->_accessMode.writeCommand(ssd1289::VERTICAL_POSITION_END,rc.X+rc.Width-1);
      this->_accessMode.writeCommand(ssd1289::SET_GDDRAM_Y,rc.X);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<LANDSCAPE,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      this->_accessMode.writeCommand(ssd1289::GATE_SCAN_POSITION,scrollPosition);
    }
  }
}

