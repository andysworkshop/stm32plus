/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8347a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of HX8347AOrientation for the panel in landscape mode.
     * @tparam TAccessMode the access mode implementation, e.g. FSMC
     */

    template<class TAccessMode>
    class HX8347AOrientation<LANDSCAPE,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        HX8347AOrientation(TAccessMode& accessMode);

        constexpr uint16_t getOrientationMemoryAccessCtrl() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const;
        void moveTo(const Rectangle& rc) const;
        void moveX(int16_t xstart,int16_t xend) const;
        void moveY(int16_t ystart,int16_t yend) const;

        void setScrollPosition(int16_t scrollPosition);
    };

    using namespace hx8347;

    /**
     * Constructor
     */

    template<class TAccessMode>
    inline HX8347AOrientation<LANDSCAPE,TAccessMode>::HX8347AOrientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for portrait mode
     * @return The entry mode register setting for landscape
     */

    template<class TAccessMode>
    constexpr inline uint16_t HX8347AOrientation<LANDSCAPE,TAccessMode>::getOrientationMemoryAccessCtrl() const {
      return MemoryAccessCtrlCmd::MV;
    }


    /**
     * Get the width in pixels
     * @return 320px
     */

    template<class TAccessMode>
    constexpr inline int16_t HX8347AOrientation<LANDSCAPE,TAccessMode>::getWidth() const {
      return 320;
    }


    /**
     * Get the height in pixels
     * @return 240px
     */

    template<class TAccessMode>
    constexpr inline int16_t HX8347AOrientation<LANDSCAPE,TAccessMode>::getHeight() const {
      return 240;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<LANDSCAPE,TAccessMode>::moveTo(const Rectangle& rc) const {

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
    inline void HX8347AOrientation<LANDSCAPE,TAccessMode>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(ColumnAddressStartCmd::OpcodeHigh,xstart >> 8);
      _accessMode.writeCommand(ColumnAddressStartCmd::OpcodeLow,xstart & 0xff);
      _accessMode.writeCommand(ColumnAddressEndCmd::OpcodeHigh,xend >> 8);
      _accessMode.writeCommand(ColumnAddressEndCmd::OpcodeLow,xend & 0xff);

      _accessMode.writeCommand(RowAddressStartCmd::OpcodeHigh,ystart >> 8);
      _accessMode.writeCommand(RowAddressStartCmd::OpcodeLow,ystart & 0xff);
      _accessMode.writeCommand(RowAddressEndCmd::OpcodeHigh,yend >> 8);
      _accessMode.writeCommand(RowAddressEndCmd::OpcodeLow,yend & 0xff);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<LANDSCAPE,TAccessMode>::moveX(int16_t xstart,int16_t xend) const {

      _accessMode.writeCommand(ColumnAddressStartCmd::OpcodeHigh,xstart >> 8);
      _accessMode.writeCommand(ColumnAddressStartCmd::OpcodeLow,xstart & 0xff);
      _accessMode.writeCommand(ColumnAddressEndCmd::OpcodeHigh,xend >> 8);
      _accessMode.writeCommand(ColumnAddressEndCmd::OpcodeLow,xend & 0xff);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<LANDSCAPE,TAccessMode>::moveY(int16_t ystart,int16_t yend) const {

      _accessMode.writeCommand(RowAddressStartCmd::OpcodeHigh,ystart >> 8);
      _accessMode.writeCommand(RowAddressStartCmd::OpcodeLow,ystart & 0xff);
      _accessMode.writeCommand(RowAddressEndCmd::OpcodeHigh,yend >> 8);
      _accessMode.writeCommand(RowAddressEndCmd::OpcodeLow,yend & 0xff);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void HX8347AOrientation<LANDSCAPE,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

      if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      _accessMode.writeCommand(SetScrollStartCmd::OpcodeHigh,(scrollPosition >> 8));
      _accessMode.writeCommand(SetScrollStartCmd::OpcodeLow,scrollPosition & 0xff);
    }
  }
}

