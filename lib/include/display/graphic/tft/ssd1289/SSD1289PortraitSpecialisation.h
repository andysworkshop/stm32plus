/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ssd1289/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of SSD1289Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class SSD1289Orientation<PORTRAIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        SSD1289Orientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationEntryMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const;
        void moveTo(const Rectangle& rc) const;
        void moveX(int16_t xstart,int16_t xend) const;
        void moveY(int16_t ystart,int16_t yend) const;

        void setScrollPosition(int16_t scrollPosition);
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline SSD1289Orientation<PORTRAIT,TAccessMode>::SSD1289Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for portrait
     */

    template<class TAccessMode>
    constexpr inline uint16_t SSD1289Orientation<PORTRAIT,TAccessMode>::getOrientationEntryMode() const {
      return 0x30;          // AM=0, ID[1..0]=11
    }


    /**
     * Get the width in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t SSD1289Orientation<PORTRAIT,TAccessMode>::getWidth() const {
      return 240;
    }


    /**
     * Get the height in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t SSD1289Orientation<PORTRAIT,TAccessMode>::getHeight() const {
      return 320;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {
      moveTo(rc.X,rc.Y,rc.X+rc.Width-1,rc.Y+rc.Height-1);
    }


    /**
     * Move the display rectangle to the rectangle described by the co-ordinates
     * @param xstart starting X position
     * @param ystart starting Y position
     * @param xend ending X position
     * @param yend ending Y position
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<PORTRAIT,TAccessMode>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(ssd1289::HORIZONTAL_POSITION,(xend << 8) | xstart);
      _accessMode.writeCommand(ssd1289::SET_GDDRAM_X,xstart);

      _accessMode.writeCommand(ssd1289::VERTICAL_POSITION_START,ystart);
      _accessMode.writeCommand(ssd1289::VERTICAL_POSITION_END,yend);
      _accessMode.writeCommand(ssd1289::SET_GDDRAM_Y,ystart);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<PORTRAIT,TAccessMode>::moveX(int16_t xstart,int16_t xend) const {
      _accessMode.writeCommand(ssd1289::HORIZONTAL_POSITION,(xend << 8) | xstart);
      _accessMode.writeCommand(ssd1289::SET_GDDRAM_X,xstart);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<PORTRAIT,TAccessMode>::moveY(int16_t ystart,int16_t yend) const {
      _accessMode.writeCommand(ssd1289::VERTICAL_POSITION_START,ystart);
      _accessMode.writeCommand(ssd1289::VERTICAL_POSITION_END,yend);
      _accessMode.writeCommand(ssd1289::SET_GDDRAM_Y,ystart);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void SSD1289Orientation<PORTRAIT,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      _accessMode.writeCommand(ssd1289::GATE_SCAN_POSITION,scrollPosition);
    }
  }
}

