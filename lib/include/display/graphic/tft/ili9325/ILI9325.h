/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/ili9325/ILI9325Colour.h"
#include "display/graphic/tft/ili9325/ILI9325Orientation.h"
#include "display/graphic/tft/ili9325/ILI9325Gamma.h"
#include "display/graphic/tft/ili9325/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic ILI9325 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    class ILI9325 : public ILI9325Colour<TColourDepth,TAccessMode>,
                    public ILI9325Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 240,
          LONG_SIDE = 320
        };

      protected:
        TAccessMode& _accessMode;

      public:
        ILI9325(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(ILI9325Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline ILI9325<TOrientation,TColourDepth,TAccessMode>::ILI9325(TAccessMode& accessMode)
      : ILI9325Colour<TColourDepth,TAccessMode>(accessMode),
        ILI9325Orientation<TOrientation,TAccessMode>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ILI9325<TOrientation,TColourDepth,TAccessMode>::initialise() const {

      // reset the device

      _accessMode.reset();

      // somewhat large power-on sequence

      _accessMode.writeCommand(ili9325::TimingCtrl1::Opcode,0x3008); // Set internal timing
      _accessMode.writeCommand(ili9325::TimingCtrl2::Opcode,0x0012); // Set internal timing
      _accessMode.writeCommand(ili9325::TimingCtrl3::Opcode,0x1231); // Set internal timing

      _accessMode.writeCommand(ili9325::StartOscillationCmd::Opcode,ili9325::StartOscillationCmd::Enable);
      _accessMode.writeCommand(ili9325::DriverOutputControlCmd::Opcode,ili9325::DriverOutputControlCmd::SS);

      // set 1 line inversion
      _accessMode.writeCommand(ili9325::DrivingWaveControlCmd::Opcode,ili9325::DrivingWaveControlCmd::FixedBits | ili9325::DrivingWaveControlCmd::EOR | ili9325::DrivingWaveControlCmd::BC);

      // no resizing
      _accessMode.writeCommand(ili9325::ResizingControlCmd::Opcode,ili9325::ResizingControlCmd::NO_RESIZING);

      // front and back porch = 2
      _accessMode.writeCommand(ili9325::DisplayCtrl2Cmd::Opcode,ili9325::DisplayCtrl2Cmd::FRONT_PORCH(2) | ili9325::DisplayCtrl2Cmd::BACK_PORCH(2));

      _accessMode.writeCommand(ili9325::DisplayCtrl3Cmd::Opcode,0);
      _accessMode.writeCommand(ili9325::DisplayCtrl4Cmd::Opcode,0);

      _accessMode.writeCommand(ili9325::RGBDisplayInterfaceCtrl1Cmd::Opcode,ili9325::RGBDisplayInterfaceCtrl1Cmd::RGB18 | ili9325::RGBDisplayInterfaceCtrl1Cmd::MODE_INTERNAL_CLOCK);

      _accessMode.writeCommand(ili9325::FrameMarkerPositionCmd::Opcode,0);

      _accessMode.writeCommand(ili9325::RGBDisplayInterfaceCtrl2Cmd::Opcode,0);

      // power On sequence

      _accessMode.writeCommand(ili9325::PowerCtrl1Cmd::Opcode,ili9325::PowerCtrl1Cmd::AP_HALT);

      _accessMode.writeCommand(ili9325::PowerCtrl2Cmd::Opcode,ili9325::PowerCtrl2Cmd::VC_100 | ili9325::PowerCtrl2Cmd::STEPUP_FREQ1_1 | ili9325::PowerCtrl2Cmd::STEPUP_FREQ2_4);

      _accessMode.writeCommand(ili9325::PowerCtrl3Cmd::Opcode,0);
      _accessMode.writeCommand(ili9325::PowerCtrl4Cmd::Opcode,0);

      // discharge capacitor power voltage
      MillisecondTimer::delay(200);

      _accessMode.writeCommand(ili9325::PowerCtrl1Cmd::Opcode,ili9325::PowerCtrl1Cmd::APE | ili9325::PowerCtrl1Cmd::AP_1_1 | ili9325::PowerCtrl1Cmd::STEP_UP_FACTOR(6) | ili9325::PowerCtrl1Cmd::SOURCE_DRIVER_ENABLE);
      _accessMode.writeCommand(ili9325::PowerCtrl2Cmd::Opcode,ili9325::PowerCtrl2Cmd::VC_100 | ili9325::PowerCtrl2Cmd::STEPUP_FREQ1_4 | ili9325::PowerCtrl2Cmd::STEPUP_FREQ2_16);

      // Delay 50ms
      MillisecondTimer::delay(50);

      // External reference voltage= Vci;
      _accessMode.writeCommand(ili9325::PowerCtrl3Cmd::Opcode,ili9325::PowerCtrl3Cmd::VRH(12) | ili9325::PowerCtrl3Cmd::PON);

      // Delay 50ms
      MillisecondTimer::delay(50);

      _accessMode.writeCommand(ili9325::PowerCtrl4Cmd::Opcode,ili9325::PowerCtrl4Cmd::VDV(0x18));

      // R29=000C when R12=009D;VCM[5:0] for VCOMH
      _accessMode.writeCommand(ili9325::PowerCtrl7Cmd::Opcode,0x1C);

      // Frame Rate = 128Hz
      _accessMode.writeCommand(ili9325::FrameRateAndColorControlCmd::Opcode,ili9325::FrameRateAndColorControlCmd::FR_128);

      // Delay 50ms
      MillisecondTimer::delay(50);

      // move the cursor to (0,0)
      _accessMode.writeCommand(ili9325::HorizontalAddressCmd::Opcode,0);
      _accessMode.writeCommand(ili9325::VerticalAddressCmd::Opcode,0);

      // Set GRAM area

      _accessMode.writeCommand(ili9325::HorizontalRAMPositionStartCmd::Opcode,0); // Horizontal GRAM Start Address
      _accessMode.writeCommand(ili9325::HorizontalRAMPositionEndCmd::Opcode,239); // Horizontal GRAM End Address
      _accessMode.writeCommand(ili9325::VerticalRAMPositionStartCmd::Opcode,0); // Vertical GRAM Start Address
      _accessMode.writeCommand(ili9325::VerticalRAMPositionEndCmd::Opcode,319); // Vertical GRAM Start Address

      // Gate scan line
      _accessMode.writeCommand(ili9325::GateScanControl1Cmd::Opcode,ili9325::GateScanControl1Cmd::NUM_LINES(39) | ili9325::GateScanControl1Cmd::GS);
      _accessMode.writeCommand(ili9325::GateScanControl2Cmd::Opcode,ili9325::GateScanControl2Cmd::REV | ili9325::GateScanControl2Cmd::VLE);
      _accessMode.writeCommand(ili9325::GateScanControlScrollCmd::Opcode,ili9325::GateScanControlScrollCmd::SCROLL(0));

      // Partial Display Control

      _accessMode.writeCommand(ili9325::PartialImage1DisplayPosition::Opcode,ili9325::PartialImage1DisplayPosition::POSITION(0));
      _accessMode.writeCommand(ili9325::PartialImage1RAMStartAddress::Opcode,ili9325::PartialImage1RAMStartAddress::STARTADDRESS(0));
      _accessMode.writeCommand(ili9325::PartialImage1RAMEndAddress::Opcode,ili9325::PartialImage1RAMEndAddress::ENDADDRESS(0));

      _accessMode.writeCommand(ili9325::PartialImage2DisplayPosition::Opcode,ili9325::PartialImage2DisplayPosition::POSITION(0));
      _accessMode.writeCommand(ili9325::PartialImage2RAMStartAddress::Opcode,ili9325::PartialImage2RAMStartAddress::STARTADDRESS(0));
      _accessMode.writeCommand(ili9325::PartialImage2RAMEndAddress::Opcode,ili9325::PartialImage2RAMEndAddress::ENDADDRESS(0));

      // Panel Control

      _accessMode.writeCommand(ili9325::PanelInterfaceControl1::Opcode,ili9325::PanelInterfaceControl1::RTNI(0x10));
      _accessMode.writeCommand(ili9325::PanelInterfaceControl2::Opcode,0);
      _accessMode.writeCommand(ili9325::PanelInterfaceControl3::Opcode,3);
      _accessMode.writeCommand(ili9325::PanelInterfaceControl4::Opcode,ili9325::PanelInterfaceControl4::RTNE(0x10) | ili9325::PanelInterfaceControl4::DIVE(1));
      _accessMode.writeCommand(ili9325::PanelInterfaceControl5::Opcode,0);
      _accessMode.writeCommand(ili9325::PanelInterfaceControl6::Opcode,0);

      // display ON

      _accessMode.writeCommand(ili9325::DisplayCtrl1Cmd::Opcode,ili9325::DisplayCtrl1Cmd::DISPLAY_ON | ili9325::DisplayCtrl1Cmd::GATE_DRIVER_NORMAL | ili9325::DisplayCtrl1Cmd::BASE_IMAGE);

      // apply entry mode by combining the values from the orientation and colour depth
      // with the common BGR setting

      _accessMode.writeCommand(ili9325::EntryModeCmd::Opcode,this->getColourEntryMode() | this->getOrientationEntryMode() | ili9325::EntryModeCmd::BGR);
    }


    /**
     * Apply the 10 panel gamma settings
     * @param gamma The collection of gamma values
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ILI9325<TOrientation,TColourDepth,TAccessMode>::applyGamma(ILI9325Gamma& gamma) const {

      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+0,gamma[0]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+1,gamma[1]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+2,gamma[2]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+3,gamma[3]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+4,gamma[4]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+5,gamma[5]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+6,gamma[6]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+7,gamma[7]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+8,gamma[8]);
      _accessMode.writeCommand(ili9325::GammaControlCmd::FirstOpcode+9,gamma[9]);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ILI9325<TOrientation,TColourDepth,TAccessMode>::sleep() const {
      _accessMode.writeCommand(ili9325::DisplayCtrl1Cmd::Opcode,ili9325::DisplayCtrl1Cmd::DISPLAY_HALT);
      _accessMode.writeCommand(ili9325::PowerCtrl1Cmd::Opcode,ili9325::PowerCtrl1Cmd::SLEEP);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ILI9325<TOrientation,TColourDepth,TAccessMode>::wake() const {
      _accessMode.writeCommand(ili9325::PowerCtrl1Cmd::Opcode,ili9325::PowerCtrl1Cmd::APE | ili9325::PowerCtrl1Cmd::AP_1_1 | ili9325::PowerCtrl1Cmd::STEP_UP_FACTOR(6) | ili9325::PowerCtrl1Cmd::SOURCE_DRIVER_ENABLE);
      _accessMode.writeCommand(ili9325::DisplayCtrl1Cmd::Opcode,ili9325::DisplayCtrl1Cmd::DISPLAY_ON | ili9325::DisplayCtrl1Cmd::GATE_DRIVER_NORMAL | ili9325::DisplayCtrl1Cmd::BASE_IMAGE);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
    inline void ILI9325<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
      _accessMode.writeCommand(ili9325::GRAMStartWritingCmd::Opcode);
    }
  }
}
