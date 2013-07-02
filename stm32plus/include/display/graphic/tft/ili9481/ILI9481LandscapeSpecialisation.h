/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ili9481/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of ILI9481Orientation for the panel in landscape mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class ILI9481Orientation<LANDSCAPE,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        ILI9481Orientation(TAccessMode& accessMode);

        constexpr uint16_t getAddressMode() const;

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
    inline ILI9481Orientation<LANDSCAPE,TAccessMode>::ILI9481Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The address mode register setting for landscape
     */

    template<class TAccessMode>
    constexpr inline uint16_t ILI9481Orientation<LANDSCAPE,TAccessMode>::getAddressMode() const {
      return ili9481::SetAddressModeCmd::PAGECOL_SELECTION;
    }


    /**
     * Get the width in pixels
     * @return 480px
     */

    template<class TAccessMode>
    constexpr inline int16_t ILI9481Orientation<LANDSCAPE,TAccessMode>::getWidth() const {
      return 480;
    }


    /**
     * Get the height in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t ILI9481Orientation<LANDSCAPE,TAccessMode>::getHeight() const {
      return 320;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<LANDSCAPE,TAccessMode>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(ili9481::SetColumnAddressCmd::Opcode,rc.X >> 8);
      this->_accessMode.writeData(rc.X & 0xff);
      this->_accessMode.writeData((rc.X+rc.Width-1) >> 8);
      this->_accessMode.writeData((rc.X+rc.Width-1) & 0xff);

      this->_accessMode.writeCommand(ili9481::SetPageAddressCmd::Opcode,rc.Y >> 8);
      this->_accessMode.writeData(rc.Y & 0xff);
      this->_accessMode.writeData((rc.Y+rc.Height-1) >> 8);
      this->_accessMode.writeData((rc.Y+rc.Height-1) & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void ILI9481Orientation<LANDSCAPE,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=480;
      else if(scrollPosition>479)
        scrollPosition-=480;

      // write to the register

      this->_accessMode.writeCommand(ili9481::SetScrollStartCmd::Opcode,(scrollPosition >> 8) & 1);
      this->_accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

