/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
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
    inline void LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(TPanelTraits::template getColumnAddressCommand<PORTRAIT>());

      _accessMode.writeData(0);         // x=0..239
      _accessMode.writeData(xstart);
      _accessMode.writeData(0);
      _accessMode.writeData(xend);

      _accessMode.writeCommand(TPanelTraits::template getRowAddressCommand<PORTRAIT>());

      _accessMode.writeData(ystart >> 8);    // y=0..319
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::moveX(int16_t xstart,int16_t xend) const {

      _accessMode.writeCommand(TPanelTraits::template getColumnAddressCommand<PORTRAIT>());

      _accessMode.writeData(0);         // x=0..239
      _accessMode.writeData(xstart);
      _accessMode.writeData(0);
      _accessMode.writeData(xend);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void LDS285Orientation<PORTRAIT,TAccessMode,TPanelTraits>::moveY(int16_t ystart,int16_t yend) const {

      _accessMode.writeCommand(TPanelTraits::template getRowAddressCommand<PORTRAIT>());

      _accessMode.writeData(ystart >> 8);    // y=0..319
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
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

      _accessMode.writeCommand(lds285::VERTICAL_SCROLLING_START_ADDRESS);
      _accessMode.writeData(scrollPosition >> 8);
      _accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

