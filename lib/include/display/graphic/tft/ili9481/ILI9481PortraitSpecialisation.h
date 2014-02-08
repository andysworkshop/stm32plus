/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ili9481/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of ILI9481Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class ILI9481Orientation<PORTRAIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        ILI9481Orientation(TAccessMode& accessMode);

        constexpr uint16_t getAddressMode() const;

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
    inline ILI9481Orientation<PORTRAIT,TAccessMode>::ILI9481Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The address mode register setting for portrait
     */

    template<class TAccessMode>
    constexpr inline uint16_t ILI9481Orientation<PORTRAIT,TAccessMode>::getAddressMode() const {
      return ili9481::SetAddressModeCmd::VERTICAL_FLIP;
    }


    /**
     * Get the width in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t ILI9481Orientation<PORTRAIT,TAccessMode>::getWidth() const {
      return 320;
    }


    /**
     * Get the height in pixels
     * @return 480px
     */

    template<class TAccessMode>
    constexpr inline int16_t ILI9481Orientation<PORTRAIT,TAccessMode>::getHeight() const {
      return 480;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {
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
    inline void ILI9481Orientation<PORTRAIT,TAccessMode>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(ili9481::SetColumnAddressCmd::Opcode,xstart >> 8);
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend >> 8);
      _accessMode.writeData(xend & 0xff);

      _accessMode.writeCommand(ili9481::SetPageAddressCmd::Opcode,ystart >> 8);
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<PORTRAIT,TAccessMode>::moveX(int16_t xstart,int16_t xend) const {
      _accessMode.writeCommand(ili9481::SetColumnAddressCmd::Opcode,xstart >> 8);
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend >> 8);
      _accessMode.writeData(xend & 0xff);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<PORTRAIT,TAccessMode>::moveY(int16_t ystart,int16_t yend) const {
      _accessMode.writeCommand(ili9481::SetPageAddressCmd::Opcode,ystart >> 8);
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<PORTRAIT,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=480;
      else if(scrollPosition>479)
        scrollPosition-=480;

      // write to the register

      _accessMode.writeCommand(ili9481::SetScrollStartCmd::Opcode,(scrollPosition >> 8) & 1);
      _accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

