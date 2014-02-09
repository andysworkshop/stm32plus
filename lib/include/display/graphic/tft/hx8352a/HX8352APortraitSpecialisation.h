/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8352a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of HX8352AOrientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode,class TPanelTraits>
    class HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        HX8352AOrientation(TAccessMode& accessMode);

        constexpr uint16_t getMemoryAccessControl() const;

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

    template<class TAccessMode,class TPanelTraits>
    inline HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::HX8352AOrientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for memory access control
     * @return The entry mode register setting for portrait
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getMemoryAccessControl() const {
      return 0xA;   // BGR | SCROLL
    }


    /**
     * Get the width in pixels
     * @return the panel's width
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getWidth() const {
      return TPanelTraits::SHORT_SIDE;
    }


    /**
     * Get the height in pixels
     * @return the panel's height
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getHeight() const {
      return TPanelTraits::LONG_SIDE;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {
      moveTo(rc.X,rc.Y,rc.X+rc.Width-1,rc.Y+rc.Height-1);
    }


    /**
     * Move the display rectangle to the rectangle described by the co-ordinates
     * @param xstart starting X position
     * @param ystart starting Y position
     * @param xend ending X position
     * @param yend ending Y position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_H,xstart >> 8);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_L,xstart & 0xff);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_H,xend >> 8);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_L,xend & 0xff);

      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_H,ystart >> 8);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_L,ystart & 0xff);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_H,yend >> 8);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_L,yend & 0xff);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::moveX(int16_t xstart,int16_t xend) const {

      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_H,xstart >> 8);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_L,xstart & 0xff);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_H,xend >> 8);
      _accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_L,xend & 0xff);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::moveY(int16_t ystart,int16_t yend) const {

      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_H,ystart >> 8);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_L,ystart & 0xff);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_H,yend >> 8);
      _accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_L,yend & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=TPanelTraits::LONG_SIDE;
      else if(scrollPosition>TPanelTraits::LONG_SIDE-1)
        scrollPosition-=TPanelTraits::LONG_SIDE;

      // write to the register

      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_START_ADDRESS_H,(scrollPosition >> 8));
      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_START_ADDRESS_L,scrollPosition & 0xff);
    }
  }
}

