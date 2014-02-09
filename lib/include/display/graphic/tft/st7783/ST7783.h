/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/st7783/ST7783Colour.h"
#include "display/graphic/tft/st7783/ST7783Orientation.h"
#include "display/graphic/tft/st7783/ST7783Gamma.h"
#include "display/graphic/tft/st7783/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic ST7783 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    class ST7783 : public ST7783Colour<TColourDepth,TAccessMode>,
                   public ST7783Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 240,
          LONG_SIDE = 320
        };

      protected:
        TAccessMode& _accessMode;

      public:
        ST7783(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(ST7783Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline ST7783<TOrientation,TColourDepth,TAccessMode>::ST7783(TAccessMode& accessMode)
      : ST7783Colour<TColourDepth,TAccessMode>(accessMode),
        ST7783Orientation<TOrientation,TAccessMode>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ST7783<TOrientation,TColourDepth,TAccessMode>::initialise() const {

      // reset the device

      _accessMode.reset();

      _accessMode.writeCommand(0x00FF,0x0001);
      _accessMode.writeCommand(0x00F3,0x0008);
      _accessMode.writeCommand(0x00F3);

      // Driver Output Control Register (R01h)
      _accessMode.writeCommand(st7783::DRIVER_OUTPUT_CONTROL,0x0100);

      // LCD Driving Waveform Control (R02h)
      _accessMode.writeCommand(st7783::LCD_DRIVING_CONTROL,0x0700);

      // Entry Mode (R03h)
      _accessMode.writeCommand(st7783::ENTRY_MODE,this->getColourEntryMode() | this->getOrientationEntryMode() | 0x1000);

      // Display control 2 (R08h)
      _accessMode.writeCommand(st7783::DISPLAY_CONTROL_2,0x0302);

      // Display Control 3 (R09h)
      _accessMode.writeCommand(st7783::DISPLAY_CONTROL_3,0x0000);

      // Power Control 1 (R10h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_1,0x0000);

      // Power Control 2 (R11h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_2,0x0007);

      // Power Control 3 (R12h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_3,0x0000);

      // Power Control 4 (R13h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_4,0x0000);
      MillisecondTimer::delay(50);

      // Power Control 1 (R10h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_1,0x14B0);
      MillisecondTimer::delay(50);

      // Power Control 2 (R11h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_2,0x0007);
      MillisecondTimer::delay(50);

      // Power Control 3 (R12h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_3,0x008E);

      // Power Control 4 (R13h)
      _accessMode.writeCommand(st7783::POWER_CONTROL_3,0x0C00);

      // VCOMH Control (R29h)
      _accessMode.writeCommand(st7783::VCOMH_CONTROL,0x0015);
      MillisecondTimer::delay(50);

      // Gate scan control (R60h)
      _accessMode.writeCommand(st7783::GATE_SCAN_CONTROL_1,0xa700);

      // Gate scan control (R60h)
      _accessMode.writeCommand(st7783::GATE_SCAN_CONTROL_2,0x0001);

      // Panel Interface Control 1 (R90h)
      _accessMode.writeCommand(st7783::PANEL_INTERFACE_CONTROL_1,0x0029);

      // Display Control 1 (R07h)
      _accessMode.writeCommand(st7783::DISPLAY_CONTROL_1,0x0133);     // Display Control (R07h)
      MillisecondTimer::delay(50);
    }


    /**
     * Apply the 10 panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ST7783<TOrientation,TColourDepth,TAccessMode>::applyGamma(ST7783Gamma& gamma) const {

      _accessMode.writeCommand(st7783::GAMMA_CONTROL_1,gamma[0]);     // Gamma Control 1
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_2,gamma[1]);     // Gamma Control 2
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_3,gamma[2]);     // Gamma Control 3
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_4,gamma[3]);     // Gamma Control 6
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_5,gamma[4]);     // Gamma Control 7
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_6,gamma[5]);     // Gamma Control 8
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_7,gamma[6]);     // Gamma Control 9
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_8,gamma[7]);     // Gamma Control 10
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_9,gamma[8]);     // Gamma Control 13
      _accessMode.writeCommand(st7783::GAMMA_CONTROL_10,gamma[9]);     // Gamma Control 14
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ST7783<TOrientation,TColourDepth,TAccessMode>::sleep() const {

      // set standby bit in POWER_CONTROL_1
      _accessMode.writeCommand(st7783::POWER_CONTROL_1,0x14B0 | 2);   // STB (standby)
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ST7783<TOrientation,TColourDepth,TAccessMode>::wake() const {

      // clear standby bit in POWER_CONTROL_1
      _accessMode.writeCommand(st7783::POWER_CONTROL_1,0x14B0);       // ~STB (wake)
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ST7783<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
      _accessMode.writeCommand(st7783::MEMORY_WRITE);
    }
  }
}
