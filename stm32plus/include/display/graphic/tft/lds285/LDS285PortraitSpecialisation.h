/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/lds285/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of LDS285Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode,class TPanelTraits>
    class LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        LDS285Orientation(TAccessMode& accessMode);

        constexpr uint16_t getMemoryAccessControl() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;
        void moveTo(const Rectangle& rc) const;

        void setScrollPosition(int16_t scrollPosition);
    };


    /**
     * Constructor
     */

    template<class TAccessMode,class TPanelTraits>
    inline LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::LDS285Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for memory access control
     * @return The entry mode register setting for portrait
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint16_t LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getMemoryAccessControl() const {
      return 0x60;
    }


    /**
     * Get the width in pixels
     * @return 240px
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getWidth() const {
      return 240;
    }


    /**
     * Get the height in pixels
     * @return 320px
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::getHeight() const {
      return 320;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode,class TPanelTraits>
    inline void LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(TPanelTraits::template getColumnAddressCommand<PORTRAIT>());

      this->_accessMode.writeData(0);         // x=0..239
      this->_accessMode.writeData(rc.X);
      this->_accessMode.writeData(0);
      this->_accessMode.writeData(rc.X+rc.Width-1);

      this->_accessMode.writeCommand(TPanelTraits::template getRowAddressCommand<PORTRAIT>());

      this->_accessMode.writeData(rc.Y >> 8);    // y=0..319
      this->_accessMode.writeData(rc.Y & 0xff);
      this->_accessMode.writeData((rc.Y+rc.Height-1)>>8);
      this->_accessMode.writeData((rc.Y+rc.Height-1) & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) {

      // pull into range

      if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      this->_accessMode.writeCommand(lds285::VERTICAL_SCROLLING_START_ADDRESS);
      this->_accessMode.writeData(scrollPosition >> 8);
      this->_accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

