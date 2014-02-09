/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/lds285/LDS285Colour.h"
#include "display/graphic/tft/lds285/LDS285Orientation.h"
#include "display/graphic/tft/lds285/LDS285Gamma.h"
#include "display/graphic/tft/lds285/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic LDS285 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    class LDS285 : public LDS285Colour<TColourDepth,TAccessMode,TPanelTraits>,
                   public LDS285Orientation<TOrientation,TAccessMode,TPanelTraits> {

      public:
        enum {
          SHORT_SIDE = 240,
          LONG_SIDE = 320,

          // CADB image types

          CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_NONE = 0,
          CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_GUI = 1,
          CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_STILL_IMAGE = 2,
          CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_MOVING_IMAGE = 3
        };

      protected:
        TAccessMode& _accessMode;
        uint8_t _backlightPercentage;

      public:
        LDS285(TAccessMode& accessMode);

        void initialise();

        void applyGamma(LDS285Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
        void setBacklight(uint8_t percentage);
        void fadeBacklightTo(uint16_t newPercentage,int msPerStep);
        void setScrollArea(uint16_t y,uint16_t height) const;
        void setContentAdaptiveImageType(uint8_t type) const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::LDS285(TAccessMode& accessMode)
      : LDS285Colour<TColourDepth,TAccessMode,TPanelTraits>(accessMode),
        LDS285Orientation<TOrientation,TAccessMode,TPanelTraits>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() {

      // reset the device

      _accessMode.reset();

      // start up the display

      _accessMode.writeCommand(lds285::SLEEP_OUT);
      MillisecondTimer::delay(10);
      _accessMode.writeCommand(lds285::DISPLAY_INVERSION_OFF);
      _accessMode.writeCommand(lds285::IDLE_MODE_OFF);
      _accessMode.writeCommand(lds285::NORMAL_DISPLAY_MODE_ON);

      // reset scroll area and position

      this->setScrollArea(0,320);
      this->setScrollPosition(0);

      // memory access control comes from the orientation specialisation

      _accessMode.writeCommand(lds285::MEMORY_ACCESS_CONTROL,this->getMemoryAccessControl());

      // interface pixel format comes from the colour specialisation

      _accessMode.writeCommand(lds285::INTERFACE_PIXEL_FORMAT,this->getInterfacePixelFormat());

      // brightness enabled for GUI and to zero to start with

      setBacklight(0);
      _accessMode.writeCommand(lds285::WRITE_CTRL_DISPLAY,0x24);
      setContentAdaptiveImageType(CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_GUI);

      // wait the required number of ms. before we can turn the display on

      MillisecondTimer::delay(125);
      _accessMode.writeCommand(lds285::DISPLAY_ON);
    }


    /**
     * Fade the backlight to a new level
     * @param newPercentage The new percentage to go to
     * @param msPerStep The number of milliseconds to wait per step to enable smooth transition
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::fadeBacklightTo(uint16_t newPercentage,int msPerStep) {

      int8_t direction;

      if(newPercentage==_backlightPercentage)
        return;

      direction=newPercentage>_backlightPercentage ? 1 : -1;

      while(newPercentage!=_backlightPercentage) {
        setBacklight(_backlightPercentage+direction);
        MillisecondTimer::delay(msPerStep);
      }
    }


    /**
     * Set a new backlight brightness level to a percentage
     * @param brightness value (0..100)
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setBacklight(uint8_t percentage) {

      uint8_t value;

      value=static_cast<uint8_t>((255*(uint16_t)percentage)/100);

        _accessMode.writeCommand(lds285::WRITE_DISPLAY_BRIGHTNESS,value);
      _backlightPercentage=percentage;
    }


    /**
     * Set a new content-adaptive-brightness image type
     * @param type CONTENT_ADAPTIVE_BRIGHTNESS_TYPE_[NONE/GUI/STILL_IMAGE/MOVING_IMAGE]
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setContentAdaptiveImageType(uint8_t type) const {
      _accessMode.writeCommand(lds285::WRITE_CONTENT_ADAPTIVE_BRIGHTNESS,type);
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The gamma class containing the one value
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(LDS285Gamma& gamma) const {

      _accessMode.writeCommand(lds285::GAMMA_SET,gamma[0] & 0xf);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {

      _accessMode.writeCommand(lds285::DISPLAY_OFF);
      _accessMode.writeCommand(lds285::SLEEP_IN);
      MillisecondTimer::delay(5);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {

      _accessMode.writeCommand(lds285::SLEEP_OUT);
      MillisecondTimer::delay(120);
      _accessMode.writeCommand(lds285::DISPLAY_ON);
      MillisecondTimer::delay(5);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
      _accessMode.writeCommand(lds285::MEMORY_WRITE);
    }


    /**
     * Set the scroll area to a full-width rectangle region
     * @param y The y-co-ord of the region
     * @param height The height of the region
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void LDS285<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setScrollArea(uint16_t y,uint16_t height) const {
      uint16_t bfa;

      bfa=320-height-y;

      _accessMode.writeCommand(lds285::VERTICAL_SCROLLING_DEFINITION);
      _accessMode.writeData(y >> 8);
      _accessMode.writeData(y & 0xff);
      _accessMode.writeData(height >> 8);
      _accessMode.writeData(height & 0xff);
      _accessMode.writeData(bfa >> 8);
      _accessMode.writeData(bfa & 0xff);
    }
  }
}
