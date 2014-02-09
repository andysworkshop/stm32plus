/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/PanelConfiguration.h"
#include "display/graphic/Rectangle.h"


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of SSD1306Orientation for the panel in LANDSCAPE mode.
     * @tparam TAccessMode the access mode implementation, e.g. SPI
     */

    template<class TAccessMode>
    class SSD1306Orientation<LANDSCAPE,TAccessMode> {

      protected:
        TAccessMode& _accessMode;
        Point _cursorPos;
        Rectangle _window;

      protected:
        SSD1306Orientation(TAccessMode& accessMode);

        void setOrientation() const;

      public:
        int16_t getWidth() const;
        int16_t getHeight() const;

        void moveTo(const Rectangle& rc);
        void setScrollPosition(int16_t scrollPosition);
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline SSD1306Orientation<LANDSCAPE,TAccessMode>::SSD1306Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Set up for portrait mode
     */

    template<class TAccessMode>
    inline void SSD1306Orientation<LANDSCAPE,TAccessMode>::setOrientation() const {
      this->_accessMode.writeCommand(0xA1);     // Set Segment Re-Map
    }


    /**
     * Get the width in pixels
     * @return 128px
     */

    template<class TAccessMode>
    inline int16_t SSD1306Orientation<LANDSCAPE,TAccessMode>::getWidth() const {
      return 128;
    }


    /**
     * Get the height in pixels
     * @return 64px
     */

    template<class TAccessMode>
    inline int16_t SSD1306Orientation<LANDSCAPE,TAccessMode>::getHeight() const {
      return 64;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void SSD1306Orientation<LANDSCAPE,TAccessMode>::moveTo(const Rectangle& rc) {

      _window=rc;
      _cursorPos=_window.getTopLeft();

      this->_accessMode.writeCommand(0x21);       // set column address
      this->_accessMode.writeCommand(rc.X);
      this->_accessMode.writeCommand(rc.X+rc.Width-1);

      this->_accessMode.writeCommand(0x22);       // set page address (8 pages, 8 bits per page = 64 pixels)
      this->_accessMode.writeCommand(rc.Y/8);
      this->_accessMode.writeCommand((rc.Y+rc.Height-1)/8);
    }


    /**
     * Set a vertical scroll position
     * @param scrollPosition The new scroll position
     */

    template<class TAccessMode>
    inline void SSD1306Orientation<LANDSCAPE,TAccessMode>::setScrollPosition(int16_t scrollPosition) {
    }
  }
}

