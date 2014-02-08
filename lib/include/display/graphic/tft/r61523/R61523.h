/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/r61523/commands/AllCommands.h"
#include "display/graphic/tft/r61523/R61523Colour.h"
#include "display/graphic/tft/r61523/R61523Orientation.h"
#include "display/graphic/tft/r61523/R61523Gamma.h"
#include "display/graphic/tft/r61523/R61523PwmBacklight.h"
#include "display/graphic/tft/r61523/panelTraits/SonyU5Vivaz_TypeA.h"
#include "display/graphic/tft/r61523/panelTraits/SonyU5Vivaz_TypeB.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic R61523 template. The user can specialise based on the desired colour
     * depth, orientation and access mode. The TPanelTraits template parameter allows
     * customisation of the driver to cope with panels with slightly different characteristics.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    class R61523 : public R61523Colour<TColourDepth,TAccessMode>,
                   public R61523Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 360,
          LONG_SIDE = 640,

          DEVICE_CODE = 0x01221523
        };


        /**
         * Possible modes for the tearing effect
         */

        enum TearingEffectMode {
          TE_VBLANK,            //!< vertical blank only
          TE_VBLANK_HBLANK,     //!< vertical and horizontal blank
        };


      protected:
        bool _enablePwmPin;
        TAccessMode& _accessMode;

      public:
        R61523(TAccessMode& accessMode,bool enablePwmPin=true);

        void initialise() const;

        void applyGamma(const R61523Gamma& gamma) const;
        void applyGamma(uint16_t command,const R61523Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;

        uint32_t readDeviceCode() const;
        void enableTearingEffect(TearingEffectMode teMode) const;
        void disableTearingEffect() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     * @param accessMode The access mode
     * @param enablePwmPin true if the PWM backlight out pin is to be enabled
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::R61523(TAccessMode& accessMode,bool enablePwmPin)
      : R61523Colour<TColourDepth,TAccessMode>(accessMode),
        R61523Orientation<TOrientation,TAccessMode>(accessMode),
        _enablePwmPin(enablePwmPin),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence. This is a nice easy one.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() const {

      typename R61523Colour<TColourDepth,TAccessMode>::UnpackedColour uc;

      // reset the device

      _accessMode.reset();

      // enable access to all the manufacturer commands

      _accessMode.writeCommand(r61523::MCAP);
      _accessMode.writeData(4);

      if(_enablePwmPin) {

        // enable the backlight PWM output pin with some default settings and a 0% duty cycle

        _accessMode.writeCommand(r61523::BACKLIGHT_CONTROL_2);
        _accessMode.writeData(0x1);         // PWMON=1
        _accessMode.writeData(0);           // BDCV=0 (off)
        _accessMode.writeData(0x3);         // 13.7kHz
        _accessMode.writeData(0x18);        // PWMWM=1, LEDPWME=1
      }

      // exit sleep mode

      _accessMode.writeCommand(r61523::SLEEP_OUT);
      MillisecondTimer::delay(120);

      // do the panel specific initialisation

      TPanelTraits::initialise(_accessMode);

      // enter normal mode

      _accessMode.writeCommand(r61523::NORMAL_DISPLAY_MODE_ON);

      // set the orientation and colour depth

      this->setOrientation();
      this->setColourDepth();

      // clear to black

      this->unpackColour(0,uc);
      this->moveTo(0,0,this->getWidth()-1,this->getHeight()-1);
      this->fillPixels(static_cast<uint32_t>(this->getWidth())*static_cast<uint32_t>(this->getHeight()),uc);

      // display on

      _accessMode.writeCommand(r61523::DISPLAY_ON);
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(const R61523Gamma& gamma) const {

      applyGamma(r61523::GAMMA_SET_A,gamma);
      applyGamma(r61523::GAMMA_SET_B,gamma);
      applyGamma(r61523::GAMMA_SET_C,gamma);
    }


    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(uint16_t command,const R61523Gamma& gamma) const {

      uint8_t i;

      _accessMode.writeCommand(command);

      // positive and negative

      for(i=0;i<2;i++) {
        _accessMode.writeData(gamma[0]);
        _accessMode.writeData(gamma[1]);
        _accessMode.writeData(gamma[3] << 4 | gamma[2]);
        _accessMode.writeData(gamma[5] << 4 | gamma[4]);
        _accessMode.writeData(gamma[6]);
        _accessMode.writeData(gamma[8] << 4 | gamma[7]);
        _accessMode.writeData(gamma[10] << 4 | gamma[9]);
        _accessMode.writeData(gamma[11]);
        _accessMode.writeData(gamma[12]);
      }
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {
      _accessMode.writeCommand(r61523::DISPLAY_OFF);
      _accessMode.writeCommand(r61523::SLEEP_IN);
      MillisecondTimer::delay(120);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {
      _accessMode.writeCommand(r61523::SLEEP_OUT);
      MillisecondTimer::delay(120);
      _accessMode.writeCommand(r61523::DISPLAY_ON);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
      _accessMode.writeCommand(r61523::MEMORY_WRITE);
    }


    /**
     * Read the device ID code. This can be used to verify that you are talking to an R61523
     * and that you've got the timings correct for read transactions.
     * @return The device ID code. It should match DEVICE_CODE.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline uint32_t R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::readDeviceCode() const {

      uint32_t deviceCode;

      // the device code is 4 bytes

      _accessMode.writeCommand(r61523::DEVICE_CODE_READ);
      _accessMode.readData();

      deviceCode=static_cast<uint32_t>(_accessMode.readData()) << 24;
      deviceCode|=static_cast<uint32_t>(_accessMode.readData()) << 16;
      deviceCode|=static_cast<uint32_t>(_accessMode.readData()) << 8;
      deviceCode|=static_cast<uint32_t>(_accessMode.readData());

      return deviceCode;
    }


    /**
     * Enable the tearing effect signal
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::enableTearingEffect(TearingEffectMode teMode) const {

      _accessMode.writeCommand(r61523::SET_TEAR_ON);
      _accessMode.writeData(teMode==TE_VBLANK ? 0 : 1);
    }


    /**
     * Disable the tearing effect signal
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    void R61523<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::disableTearingEffect() const {
      _accessMode.writeCommand(r61523::SET_TEAR_OFF);
    }
  }
}
