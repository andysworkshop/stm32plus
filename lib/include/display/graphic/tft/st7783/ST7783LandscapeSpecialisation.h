/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/st7783/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of ST7783Orientation for the panel in landscape mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class ST7783Orientation<LANDSCAPE,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        ST7783Orientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationEntryMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const;
        void moveTo(const Rectangle& rc) const;
        void moveX(int16_t xstart,int16_t xend) const;
        void moveY(int16_t ystart,int16_t yend) const;
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline ST7783Orientation<LANDSCAPE,TAccessMode>::ST7783Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for landscape
     */

    template<class TAccessMode>
    constexpr inline uint16_t ST7783Orientation<LANDSCAPE,TAccessMode>::getOrientationEntryMode() const {
      using namespace st7783;
      return AM | ID1;
    }


    /**
     * Get the width in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t ST7783Orientation<LANDSCAPE,TAccessMode>::getWidth() const {
      return 320;
    }


    /**
     * Get the height in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t ST7783Orientation<LANDSCAPE,TAccessMode>::getHeight() const {
      return 240;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void ST7783Orientation<LANDSCAPE,TAccessMode>::moveTo(const Rectangle& rc) const {
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
    inline void ST7783Orientation<LANDSCAPE,TAccessMode>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_START,239-yend);
      _accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_END,239-ystart);

      _accessMode.writeCommand(st7783::VERTICAL_ADDRESS_START,xstart);
      _accessMode.writeCommand(st7783::VERTICAL_ADDRESS_END,xend);

      _accessMode.writeCommand(st7783::RAM_HORIZONTAL_ADDRESS,ystart);
      _accessMode.writeCommand(st7783::RAM_VERTICAL_ADDRESS,xstart);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode>
    inline void ST7783Orientation<LANDSCAPE,TAccessMode>::moveX(int16_t xstart,int16_t xend) const {
      _accessMode.writeCommand(st7783::VERTICAL_ADDRESS_START,xstart);
      _accessMode.writeCommand(st7783::VERTICAL_ADDRESS_END,xend);
      _accessMode.writeCommand(st7783::RAM_VERTICAL_ADDRESS,xstart);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode>
    inline void ST7783Orientation<LANDSCAPE,TAccessMode>::moveY(int16_t ystart,int16_t yend) const {
      _accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_START,239-yend);
      _accessMode.writeCommand(st7783::HORIZONTAL_ADDRESS_END,239-ystart);
      _accessMode.writeCommand(st7783::RAM_HORIZONTAL_ADDRESS,ystart);
    }
  }
}

