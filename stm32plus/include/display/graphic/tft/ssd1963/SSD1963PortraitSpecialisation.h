/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ssd1963/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of SSD1963Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode,class TPanelTraits>
    class SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        SSD1963Orientation(TAccessMode& accessMode);

        constexpr uint8_t getAddressMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(const Rectangle& rc) const;
        void setScrollPosition(int16_t scrollPosition) const;
    };


    /**
     * Constructor
     * @param accessMode the access mode
     */

    template<class TAccessMode,class TPanelTraits>
    inline SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::SSD1963Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for the set_address_mode function
     * @return The correct setting for portrait
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint8_t SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getAddressMode() const {
      return 0x22;
    }


    /**
     * Get the width in pixels
     * @return TPanelTraits::SHORT_SIDE
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getWidth() const {
      return TPanelTraits::SHORT_SIDE;
    }


    /**
     * Get the height in pixels
     * @return TPanelTraits::LONG_SIDE
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getHeight() const {
      return TPanelTraits::LONG_SIDE;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(ssd1963::PAGE_ADDRESS_SET);

      this->_accessMode.writeData(rc.X>>8);         // x=0..SHORT_SIDE-1
      this->_accessMode.writeData(rc.X);
      this->_accessMode.writeData((rc.X+rc.Width-1)>>8);
      this->_accessMode.writeData(rc.X+rc.Width-1);

      this->_accessMode.writeCommand(ssd1963::COLUMN_ADDRESS_SET);

      this->_accessMode.writeData(rc.Y >> 8);       // y=0..LONG_SIDE-1
      this->_accessMode.writeData(rc.Y & 0xff);
      this->_accessMode.writeData((rc.Y+rc.Height-1)>>8);
      this->_accessMode.writeData((rc.Y+rc.Height-1) & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<PORTRAIT,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) const {

      // pull into range

      if(scrollPosition<0)
        scrollPosition+=TPanelTraits::SHORT_SIDE;
      else if(scrollPosition>=TPanelTraits::SHORT_SIDE)
        scrollPosition-=TPanelTraits::SHORT_SIDE;

      if(scrollPosition>0)
        scrollPosition=TPanelTraits::SHORT_SIDE-scrollPosition;

      // write to the register

      this->_accessMode.writeCommand(ssd1963::VERTICAL_SCROLLING_START_ADDRESS);
      this->_accessMode.writeData(scrollPosition >> 8);
      this->_accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

