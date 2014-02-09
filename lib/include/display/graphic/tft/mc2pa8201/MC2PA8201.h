/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/mc2pa8201/MC2PA8201Colour.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201Orientation.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201Gamma.h"
#include "display/graphic/tft/mc2pa8201/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic MC2PA8201 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    class MC2PA8201 : public MC2PA8201Colour<TColourDepth,TAccessMode,TPanelTraits>,
                      public MC2PA8201Orientation<TOrientation,TAccessMode,TPanelTraits> {

      public:
        enum {
          SHORT_SIDE = 240,
          LONG_SIDE = 320
        };

      protected:
        TAccessMode& _accessMode;

      public:
        MC2PA8201(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(MC2PA8201Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
        void setScrollArea(uint16_t y,uint16_t height) const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::MC2PA8201(TAccessMode& accessMode)
      : MC2PA8201Colour<TColourDepth,TAccessMode,TPanelTraits>(accessMode),
        MC2PA8201Orientation<TOrientation,TAccessMode,TPanelTraits>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() const {

      // reset the device

      _accessMode.reset();

      // start up the display

      _accessMode.writeCommand(mc2pa8201::SLEEP_OUT);
      MillisecondTimer::delay(10);
      _accessMode.writeCommand(mc2pa8201::DISPLAY_INVERSION_OFF);
      _accessMode.writeCommand(mc2pa8201::IDLE_MODE_OFF);
      _accessMode.writeCommand(mc2pa8201::NORMAL_DISPLAY_MODE_ON);

      // interface pixel format comes from the colour specialisation

      _accessMode.writeCommand(mc2pa8201::INTERFACE_PIXEL_FORMAT,this->getInterfacePixelFormat());

      // memory access control comes from the orientation specialisation

      _accessMode.writeCommand(mc2pa8201::MEMORY_ACCESS_CONTROL,this->getMemoryAccessControl());

      // reset the scrolling area

      setScrollArea(0,TPanelTraits::getScrollHeight());

      // wait the required number of ms. before we can turn the display on

      MillisecondTimer::delay(125);
      _accessMode.writeCommand(mc2pa8201::DISPLAY_ON);
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The gamma class containing the one value
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(MC2PA8201Gamma& gamma) const {

      _accessMode.writeCommand(mc2pa8201::GAMMA_SET,gamma[0] & 0xf);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {

      _accessMode.writeCommand(mc2pa8201::DISPLAY_OFF);
      _accessMode.writeCommand(mc2pa8201::SLEEP_IN);
      MillisecondTimer::delay(5);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {

      _accessMode.writeCommand(mc2pa8201::SLEEP_OUT);
      MillisecondTimer::delay(120);
      _accessMode.writeCommand(mc2pa8201::DISPLAY_ON);
      MillisecondTimer::delay(5);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
      _accessMode.writeCommand(mc2pa8201::MEMORY_WRITE);
    }


    /**
     * Set the scroll area to a full-width rectangle region
     * @param y The y-co-ord of the region
     * @param height The height of the region
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void MC2PA8201<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setScrollArea(uint16_t y,uint16_t height) const {
      uint16_t bfa;

      bfa=TPanelTraits::getScrollHeight()-height-y;

      _accessMode.writeCommand(mc2pa8201::VERTICAL_SCROLLING_DEFINITION);
      _accessMode.writeData(y >> 8);
      _accessMode.writeData(y & 0xff);
      _accessMode.writeData(height >> 8);
      _accessMode.writeData(height & 0xff);
      _accessMode.writeData(bfa >> 8);
      _accessMode.writeData(bfa & 0xff);
    }
  }
}
