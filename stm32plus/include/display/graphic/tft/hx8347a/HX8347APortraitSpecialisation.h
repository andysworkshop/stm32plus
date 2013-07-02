/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8347a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of HX8347AOrientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class HX8347AOrientation<PORTRAIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        HX8347AOrientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationMemoryAccessCtrl() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;
        void moveTo(const Rectangle& rc) const;

        void setScrollPosition(int16_t scrollPosition);
    };

    using namespace hx8347;

    /**
     * Constructor
     */

    template<class TAccessMode>
    inline HX8347AOrientation<PORTRAIT,TAccessMode>::HX8347AOrientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for portrait
     */

    template<class TAccessMode>
    constexpr inline uint16_t HX8347AOrientation<PORTRAIT,TAccessMode>::getOrientationMemoryAccessCtrl() const {
      return MemoryAccessCtrlCmd::MX;
    }


    /**
     * Get the width in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t HX8347AOrientation<PORTRAIT,TAccessMode>::getWidth() const {
      return 240;
    }


    /**
     * Get the height in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t HX8347AOrientation<PORTRAIT,TAccessMode>::getHeight() const {
      return 320;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {

      this->_accessMode.writeCommand(ColumnAddressStartCmd::OpcodeHigh,rc.X >> 8);
      this->_accessMode.writeCommand(ColumnAddressStartCmd::OpcodeLow,rc.X & 0xff);
      this->_accessMode.writeCommand(ColumnAddressEndCmd::OpcodeHigh,(rc.X+rc.Width-1) >> 8);
      this->_accessMode.writeCommand(ColumnAddressEndCmd::OpcodeLow,(rc.X+rc.Width-1) & 0xff);

      this->_accessMode.writeCommand(RowAddressStartCmd::OpcodeHigh,rc.Y >> 8);
      this->_accessMode.writeCommand(RowAddressStartCmd::OpcodeLow,rc.Y & 0xff);
      this->_accessMode.writeCommand(RowAddressEndCmd::OpcodeHigh,(rc.Y+rc.Height-1) >> 8);
      this->_accessMode.writeCommand(RowAddressEndCmd::OpcodeLow,(rc.Y+rc.Height-1) & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<PORTRAIT,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      this->_accessMode.writeCommand(SetScrollStartCmd::OpcodeHigh,(scrollPosition >> 8));
      this->_accessMode.writeCommand(SetScrollStartCmd::OpcodeLow,scrollPosition & 0xff);
    }
  }
}

