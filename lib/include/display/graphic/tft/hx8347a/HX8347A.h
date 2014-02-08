/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/hx8347a/HX8347AColour.h"
#include "display/graphic/tft/hx8347a/HX8347AOrientation.h"
#include "display/graphic/tft/hx8347a/HX8347AGamma.h"
#include "display/graphic/tft/hx8347a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic HX8347A template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    class HX8347A : public HX8347AColour<TColourDepth,TAccessMode>,
                    public HX8347AOrientation<TOrientation,TAccessMode> {

      protected:
        TAccessMode& _accessMode;

      public:
        HX8347A(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(HX8347AGamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline HX8347A<TOrientation,TColourDepth,TAccessMode>::HX8347A(TAccessMode& accessMode)
      : HX8347AColour<TColourDepth,TAccessMode>(accessMode),
        HX8347AOrientation<TOrientation,TAccessMode>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void HX8347A<TOrientation,TColourDepth,TAccessMode>::initialise() const {

      // reset the device

      _accessMode.reset();

      // SAP=0111 1111
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode8,0x7F);

      // IDMON=0, INVON=1, NORON=1, PTLON=0
      _accessMode.writeCommand(hx8347::DisplayModeCmd::Opcode,hx8347::DisplayModeCmd::INVON | hx8347::DisplayModeCmd::NORON);

      // cycle control

      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode1,0x95); // N_DC=1001 0101
      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode2,0x95); // P_DC=1001 0101
      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode3,0xFF); // I_DC=1111 1111

      // display control

      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodeNBP,0x06); // N_BP=0000 0110
      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodeNFP,0x06); // N_FP=0000 0110
      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodePIB,0x06); // P_BP=0000 0110
      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodePIF,0x06); // P_FP=0000 0110
      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodeIBP,0x06); // I_BP=0000 0110
      _accessMode.writeCommand(hx8347::DisplayControlCmd::OpcodeIFP,0x06); // I_FP=0000 0110

      // cycle control

      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode4,0x01); // N_RTN=0000, N_NW=001
      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode5,0x01); // P_RTN=0000, P_NW=001
      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode6,0xF0); // I_RTN=1111, I_NW=000
      _accessMode.writeCommand(hx8347::CycleControlCmd::Opcode7,0x00); // DIV=00

      MillisecondTimer::delay(20);

      // Power Supply Setting

      _accessMode.writeCommand(hx8347::OscControlCmd::Opcode1,0x49); // OSCADJ=10 0000, OSD_EN=1 //60Hz
      _accessMode.writeCommand(hx8347::OscControlCmd::Opcode3,0x0C); // RADJ=1100,

      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode6,0x40); // BT=0100
      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode3,0x07); // VC1=111
      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode4,0x00); // VC3=000
      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode5,0x04); // VRH=0100          4.12V

      _accessMode.writeCommand(hx8347::VcomControlCmd::Opcode2,0x4D); // VCM=101 0000   3.21V
      _accessMode.writeCommand(hx8347::VcomControlCmd::Opcode3,0x11); // VDV=1 0001           -1.19V

      MillisecondTimer::delay(10);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode2,0x04); // AP=100

      MillisecondTimer::delay(20);

      _accessMode.writeCommand(hx8347::VcomControlCmd::Opcode1,0x80); //set VCOMG=1

      MillisecondTimer::delay(5);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode1,0x18); // GASENB=0, PON=1, DK=1, XDK=0, DDVDH_TRI=0, STB=0

      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode1,0x10); // GASENB=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0

      MillisecondTimer::delay(40);

      // Display ON Setting

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x04); // GON=0, DTE=0, D=01

      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x24); // GON=1, DTE=0, D=01
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x2C); // GON=1, DTE=0, D=11

      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x3C); // GON=1, DTE=1, D=11

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode9,0x38); // EQS=38h
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode10,0x78); // EQP=78h

      _accessMode.writeCommand(hx8347::CycleControlCmd::CycleControl5Opcode,0x38); // SON=38h

      _accessMode.writeCommand(hx8347::CycleControlCmd::CycleControl6Opcode,0x0F); // GDON=0Fh
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode14,0xF0); // GDOFF

      // Set spulse & rpulse
      _accessMode.writeCommand(hx8347::InternalUseCmd::Opcode3,0x02); // Test mode='1'
      _accessMode.writeCommand(hx8347::InternalUseCmd::Opcode2,0x84); // set Rpulse='1000',spulse='0100'
      _accessMode.writeCommand(hx8347::InternalUseCmd::Opcode3,0x00); // Test mode= '0'

      MillisecondTimer::delay(20);

      // apply entry mode by combining the values from the orientation and colour depth
      // with the common BGR setting

      _accessMode.writeCommand(hx8347::MemoryAccessCtrlCmd::Opcode,
          this->getOrientationMemoryAccessCtrl() | hx8347::MemoryAccessCtrlCmd::MY | hx8347::MemoryAccessCtrlCmd::BGR);

      // set up the scroll parameters

      _accessMode.writeCommand(hx8347::ScrollTopFixedAreaCmd::OpcodeHigh,0);
      _accessMode.writeCommand(hx8347::ScrollTopFixedAreaCmd::OpcodeLow,0);
      _accessMode.writeCommand(hx8347::ScrollBottomFixedAreaCmd::OpcodeHigh,0);
      _accessMode.writeCommand(hx8347::ScrollBottomFixedAreaCmd::OpcodeLow,0);
      _accessMode.writeCommand(hx8347::ScrollAreaCmd::OpcodeHigh,320 >> 8);
      _accessMode.writeCommand(hx8347::ScrollAreaCmd::OpcodeLow,320 & 0xff);

      _accessMode.writeCommand(hx8347::SetScrollStartCmd::OpcodeHigh,0);
      _accessMode.writeCommand(hx8347::SetScrollStartCmd::OpcodeLow,0);
      _accessMode.writeCommand(hx8347::GateScanControlCmd::Opcode,hx8347::GateScanControlCmd::SCROLL_ON);
    }


    /**
     * Apply the 12 panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void HX8347A<TOrientation,TColourDepth,TAccessMode>::applyGamma(HX8347AGamma& gamma) const {

      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+0,gamma[0]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+1,gamma[1]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+2,gamma[2]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+3,gamma[3]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+4,gamma[4]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+5,gamma[5]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+6,gamma[6]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+7,gamma[7]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+8,gamma[8]);
      _accessMode.writeCommand(hx8347::GammaControlCmd::FirstOpcode+9,gamma[9]);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void HX8347A<TOrientation,TColourDepth,TAccessMode>::sleep() const {

        // GON=1, DTE=1, D=10
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x38);
      MillisecondTimer::delay(40);

      // GON=1, DTE=0, D=10
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x28);
      MillisecondTimer::delay(40);

      // GON=0, DTE=0, D=00
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0);
      MillisecondTimer::delay(40);

      // SAP=0
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode8,0);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode2,0); // AP=000
      MillisecondTimer::delay(20);

      // STB=1, DK=1
      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode1,9);
      // OSC_EN=0
      _accessMode.writeCommand(hx8347::OscControlCmd::Opcode1,0);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void HX8347A<TOrientation,TColourDepth,TAccessMode>::wake() const {

      // OSCADJ=10 0000, OSD_EN=1 //60Hz
      _accessMode.writeCommand(hx8347::OscControlCmd::Opcode1,0x49);
      MillisecondTimer::delay(20);

      // GASENB=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode1,0x10);

      _accessMode.writeCommand(hx8347::PowerControlCmd::Opcode2,0x04); // AP=100
      MillisecondTimer::delay(20);

      // SAP=0111 1111
      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode8,0x7F);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x04); // GON=0, DTE=0, D=01
      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x24); // GON=1, DTE=0, D=01
      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x2C); // GON=1, DTE=0, D=11
      MillisecondTimer::delay(40);

      _accessMode.writeCommand(hx8347::DisplayControlCmd::Opcode1,0x3C); // GON=1, DTE=1, D=11
      MillisecondTimer::delay(40);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void HX8347A<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
      _accessMode.writeCommand(hx8347::WriteDataCmd::Opcode);
    }
  }
}
