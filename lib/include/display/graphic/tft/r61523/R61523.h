/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/r61523/R61523Colour.h"
#include "display/graphic/tft/r61523/R61523Orientation.h"
#include "display/graphic/tft/r61523/R61523Gamma.h"
#include "display/graphic/tft/r61523/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic R61523 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    class R61523 : public R61523Colour<TColourDepth,TAccessMode>,
                   public R61523Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 360,
          LONG_SIDE = 640
        };

      protected:
        TAccessMode& _accessMode;

      public:
        R61523(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(R61523Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline R61523<TOrientation,TColourDepth,TAccessMode>::R61523(TAccessMode& accessMode)
      : R61523Colour<TColourDepth,TAccessMode>(accessMode),
        R61523Orientation<TOrientation,TAccessMode>(accessMode),
      _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence. This is a nice easy one.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void R61523<TOrientation,TColourDepth,TAccessMode>::initialise() const {

      typename R61523Colour<TColourDepth,TAccessMode>::UnpackedColour uc;

      // reset the device

      _accessMode.reset();

      // exit sleep mode

      _accessMode.writeCommand(r61523::SLEEP_OUT);
      MillisecondTimer::delay(120);

      // clear to black

      this->unpackColour(0,uc);
      this->moveTo(0,0,this->getWidth()-1,this->getHeight()-1);
      this->fillPixels(static_cast<uint32_t>(this->getWidth())*static_cast<uint32_t>(this->getHeight()),uc);

      // set the orientation

      _accessMode.writeCommand(r61523::SET_ADDRESS_MODE,this->getAddressMode());

      // set the colour depth

      _accessMode.writeCommand(r61523::SET_PIXEL_FORMAT,this->getPixelFormat());

      // display on

      _accessMode.writeCommand(r61523::DISPLAY_ON);
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void R61523<TOrientation,TColourDepth,TAccessMode>::applyGamma(R61523Gamma& gamma) const {
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void R61523<TOrientation,TColourDepth,TAccessMode>::sleep() const {
      _accessMode.writeCommand(r61523::DISPLAY_OFF);
      _accessMode.writeCommand(r61523::SLEEP_IN);
      MillisecondTimer::delay(120);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void R61523<TOrientation,TColourDepth,TAccessMode>::wake() const {
      _accessMode.writeCommand(r61523::SLEEP_OUT);
      MillisecondTimer::delay(120);
      _accessMode.writeCommand(r61523::DISPLAY_ON);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void R61523<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
      _accessMode.writeCommand(r61523::MEMORY_WRITE);
    }
  }
}
