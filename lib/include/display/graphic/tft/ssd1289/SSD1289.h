/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/ssd1289/SSD1289Colour.h"
#include "display/graphic/tft/ssd1289/SSD1289Orientation.h"
#include "display/graphic/tft/ssd1289/SSD1289Gamma.h"
#include "display/graphic/tft/ssd1289/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic SSD1289 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    class SSD1289 : public SSD1289Colour<TColourDepth,TAccessMode>,
                    public SSD1289Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 240,
          LONG_SIDE = 320
        };

      protected:
        TAccessMode& _accessMode;

      public:
        SSD1289(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(SSD1289Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
        void beginReading() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline SSD1289<TOrientation,TColourDepth,TAccessMode>::SSD1289(TAccessMode& accessMode)
      : SSD1289Colour<TColourDepth,TAccessMode>(accessMode),
        SSD1289Orientation<TOrientation,TAccessMode>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::initialise() const {

      // reset the device

      _accessMode.reset();

      // init sequence: datashee 15.5 "Display ON Sequence"

      // GON = 1, DTE = 0, D[1:0] = 01
      _accessMode.writeCommand(ssd1289::DISPLAY_CONTROL,0x21);

      // oscillator ON
      _accessMode.writeCommand(ssd1289::OSCILLATOR,0x01);

      // GON = 1, DTE = 0, D[1:0] = 11
      _accessMode.writeCommand(ssd1289::DISPLAY_CONTROL,0x23);

      // Exit sleep mode
      _accessMode.writeCommand(ssd1289::SLEEP_MODE,0x00);

      MillisecondTimer::delay(35);

      // GON = 1, DTE = 1, D[1:0] = 11
      _accessMode.writeCommand(ssd1289::DISPLAY_CONTROL,0x33);

      // BGR etc.
      _accessMode.writeCommand(ssd1289::DRIVER_OUTPUT_CONTROL,0x13f | 0x2000 | 0x800 | 0x200);

      // entry mode: colour and orientation
      _accessMode.writeCommand(ssd1289::ENTRY_MODE,this->getColourEntryMode() | this->getOrientationEntryMode());

      // driving waveform control
      _accessMode.writeCommand(ssd1289::DRIVING_WAVEFORM_CONTROL,0x600);
    }


    /**
     * Apply the 10 panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::applyGamma(SSD1289Gamma& gamma) const {

      _accessMode.writeCommand(ssd1289::GAMMA_0,gamma[0]);
      _accessMode.writeCommand(ssd1289::GAMMA_1,gamma[1]);
      _accessMode.writeCommand(ssd1289::GAMMA_2,gamma[2]);
      _accessMode.writeCommand(ssd1289::GAMMA_3,gamma[3]);
      _accessMode.writeCommand(ssd1289::GAMMA_4,gamma[4]);
      _accessMode.writeCommand(ssd1289::GAMMA_5,gamma[5]);
      _accessMode.writeCommand(ssd1289::GAMMA_6,gamma[6]);
      _accessMode.writeCommand(ssd1289::GAMMA_7,gamma[7]);
      _accessMode.writeCommand(ssd1289::GAMMA_8,gamma[8]);
      _accessMode.writeCommand(ssd1289::GAMMA_9,gamma[9]);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::sleep() const {
      _accessMode.writeCommand(ssd1289::SLEEP_MODE,0x01);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::wake() const {
      _accessMode.writeCommand(ssd1289::SLEEP_MODE,0x00);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
      _accessMode.writeCommand(ssd1289::GRAM_WRITE_DATA);
    }


    /**
     * Issue the command that allows graphics ram reading to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void SSD1289<TOrientation,TColourDepth,TAccessMode>::beginReading() const {
      _accessMode.writeCommand(ssd1289::GRAM_WRITE_DATA);   // yes it's the same as for writing
    }
  }
}
