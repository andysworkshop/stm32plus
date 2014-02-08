/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ssd1963/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of SSD1963Orientation for the panel in LANDSCAPE mode.
     * @tparam TAccessMode the access mode implementation, e.g. one of the FSMC access modes
     */

    template<class TAccessMode,class TPanelTraits>
    class SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        SSD1963Orientation(TAccessMode& accessMode);

        constexpr uint8_t getAddressMode() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const;
        void moveTo(const Rectangle& rc) const;
        void moveX(int16_t xstart,int16_t xend) const;
        void moveY(int16_t ystart,int16_t yend) const;

        void setScrollPosition(int16_t scrollPosition) const;
    };


    /**
     * Constructor
     * @param accessMode The access mode implementation
     */

    template<class TAccessMode,class TPanelTraits>
    inline SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::SSD1963Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for the set_address_mode function
     * @return The correct setting for landscape
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint8_t SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getAddressMode() const {
      return 0x3;
    }


    /**
     * Get the width in pixels
     * @return TPanelTraits::LONG_SIDE
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getWidth() const {
      return TPanelTraits::LONG_SIDE;
    }


    /**
     * Get the height in pixels
     * @return TPanelTraits::SHORT_SIDE
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline int16_t SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getHeight() const {
      return TPanelTraits::SHORT_SIDE;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {
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
    inline void SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(ssd1963::COLUMN_ADDRESS_SET);

      _accessMode.writeData(xstart >> 8);         // x=0..LONG_SIDE-1
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend>>8);
      _accessMode.writeData(xend & 0xff);

      _accessMode.writeCommand(ssd1963::PAGE_ADDRESS_SET);

      _accessMode.writeData(ystart >>8);          // y=0..SHORT_SIDE-1
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend>>8);
      _accessMode.writeData(yend);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::moveX(int16_t xstart,int16_t xend) const {
      _accessMode.writeCommand(ssd1963::COLUMN_ADDRESS_SET);

      _accessMode.writeData(xstart >> 8);         // x=0..LONG_SIDE-1
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend>>8);
      _accessMode.writeData(xend & 0xff);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::moveY(int16_t ystart,int16_t yend) const {
      _accessMode.writeCommand(ssd1963::PAGE_ADDRESS_SET);

      _accessMode.writeData(ystart >>8);          // y=0..SHORT_SIDE-1
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend>>8);
      _accessMode.writeData(yend);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) const {

      // pull into range

      if(scrollPosition<0)
        scrollPosition+=TPanelTraits::SHORT_SIDE;
      else if(scrollPosition>=TPanelTraits::SHORT_SIDE)
        scrollPosition-=TPanelTraits::SHORT_SIDE;

      if(scrollPosition>0)
        scrollPosition=TPanelTraits::SHORT_SIDE-scrollPosition;

      // write to the register

      _accessMode.writeCommand(ssd1963::VERTICAL_SCROLLING_START_ADDRESS);
      _accessMode.writeData(scrollPosition >> 8);
      _accessMode.writeData(scrollPosition & 0xff);
    }
  }
}

