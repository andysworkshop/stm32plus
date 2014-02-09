/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/hx8352a/HX8352AColour.h"
#include "display/graphic/tft/hx8352a/HX8352AOrientation.h"
#include "display/graphic/tft/hx8352a/HX8352AGamma.h"
#include "display/graphic/tft/hx8352a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic HX8352A template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    class HX8352A : public HX8352AColour<TColourDepth,TAccessMode,TPanelTraits>,
                   public HX8352AOrientation<TOrientation,TAccessMode,TPanelTraits> {

      protected:
        TAccessMode& _accessMode;
        uint8_t _backlightPercentage;

      public:
        HX8352A(TAccessMode& accessMode);

        void initialise();

        void applyGamma(HX8352AGamma& gamma) const;
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
    inline HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::HX8352A(TAccessMode& accessMode)
      : HX8352AColour<TColourDepth,TAccessMode,TPanelTraits>(accessMode),
        HX8352AOrientation<TOrientation,TAccessMode,TPanelTraits>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() {

      // reset the device

      _accessMode.reset();

      // initialise the panel - the HX8352A supports multiple panel types hence the reset sequence
      // is not generic and is moved into the panel traits class

      TPanelTraits::initialise(_accessMode);

      // set the orientation and scroll area

      _accessMode.writeCommand(hx8352a::MEMORY_ACCESS_CONTROL,this->getMemoryAccessControl());
      setScrollArea(0,TPanelTraits::LONG_SIDE);
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The gamma class containing the one value
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(HX8352AGamma& gamma) const {
      _accessMode.writeCommand(hx8352a::GAMMA_1+0,gamma[0]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+1,gamma[1]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+2,gamma[2]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+3,gamma[3]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+4,gamma[4]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+5,gamma[5]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+6,gamma[6]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+7,gamma[7]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+8,gamma[8]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+9,gamma[9]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+10,gamma[10]);
      _accessMode.writeCommand(hx8352a::GAMMA_1+11,gamma[11]);
    }


    /**
     * Send the panel to sleep. This procedure follows the specification
     * in the Himax application note.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {

      // display Off
      _accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x38);      //GON=1, DTE=1, D=10
      MillisecondTimer::delay (40);

      _accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x28);      //GON=1, DTE=0, D=10
      MillisecondTimer::delay (40);

      _accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x00);      //GON=0, DTE=0, D=00

      // power Off
      _accessMode.writeCommand(hx8352a::POWER_CONTROL_6,0x14);      // VCOMG=0, VDV=1_0100
      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x02);    // GASENB=0, PON=0, DK=0, XDK=0, VLCD_TRI=1, STB=0
      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x0A);    // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=1, STB=0
      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8352a::POWER_CONTROL_3,0x40);    // AP=000
      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8352a::SOURCE_CONTROL_1,0x00);   // N_SAP=1100 0000
      MillisecondTimer::delay(10);

      // into STB mode
      _accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x0B);    // GASENB=0, PON=0, DK=0, XDK=0, VLCD_TRI=1, STB=1
      MillisecondTimer::delay(10);

      // stop oscillation
      _accessMode.writeCommand(hx8352a::OSC_CONTROL_1,0x90);      // RADJ=1001, OSC_EN=0
    }


    /**
     * Wake the panel up. The wakeup sequence needs to set the power and
     * oscillation parameters which are panel-specific. Hence we defer to
     * the panel traits for the implementation.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {
      TPanelTraits::wake(_accessMode);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
      _accessMode.writeCommand(hx8352a::MEMORY_WRITE);
    }


    /**
     * Set the scroll area to a full-width rectangle region
     * @param y The y-co-ord of the region
     * @param height The height of the region
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setScrollArea(uint16_t y,uint16_t height) const {

      uint16_t bfa;

      bfa=TPanelTraits::LONG_SIDE-height-y;

      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_TOP_FIXED_AREA_H,y >> 8);
      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_TOP_FIXED_AREA_L,y & 0xff);

      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_HEIGHT_H,height >> 8);
      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_HEIGHT_L,height & 0xff);

      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_BOTTOM_FIXED_AREA_H,bfa >> 8);
      _accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_BOTTOM_FIXED_AREA_L,bfa & 0xff);
    }
  }
}
