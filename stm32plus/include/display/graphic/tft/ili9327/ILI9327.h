/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/ili9327/ILI9327Colour.h"
#include "display/graphic/tft/ili9327/ILI9327Orientation.h"
#include "display/graphic/tft/ili9327/ILI9327Gamma.h"
#include "display/graphic/tft/ili9327/commands/AllCommands.h"


namespace stm32plus {
	namespace display {

		/**
		 * Generic ILI9327 template. The user can specialise based on the desired colour
		 * depth, orientation and access mode.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		class ILI9327 : public ILI9327Colour<TColourDepth,TAccessMode>,
										public ILI9327Orientation<TOrientation,TAccessMode,TPanelTraits> {

			protected:
				TAccessMode& _accessMode;

			public:
				ILI9327(TAccessMode& accessMode);

				void initialise() const;

				void applyGamma(ILI9327Gamma& gamma) const;
				void sleep() const;
				void wake() const;
				void beginWriting() const;
		};


		/**
		 * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
		 * common base class for use by all.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::ILI9327(TAccessMode& accessMode)
			: ILI9327Colour<TColourDepth,TAccessMode>(accessMode),
			  ILI9327Orientation<TOrientation,TAccessMode,TPanelTraits>(accessMode),
			  _accessMode(accessMode) {
		}


		/**
		 * Initialise the LCD. Do the reset sequence.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() const {

			// reset the device

			this->_accessMode.reset();

			// power setting: we use the internal ref vci=2.5v

			this->_accessMode.writeCommand(ili9327::PowerSettingCmd::Opcode,ili9327::PowerSettingCmd::VC_100); // vci1=vci*1 (vci=2.5V therefore vci1=2.5V)
			this->_accessMode.writeData(ili9327::PowerSettingCmd::BT_5_4); // VGH=vci1*5, VGL=vci1*-4
			this->_accessMode.writeData(ili9327::PowerSettingCmd::VRH(0xc) | ili9327::PowerSettingCmd::VCIRE); // int ref + VREG1OUT = vci1*1.9 = 4.75

			// VCom (more power settings)

			this->_accessMode.writeCommand(ili9327::VComCmd::Opcode,0); // register D1 for setting VCom
			this->_accessMode.writeData(ili9327::VComCmd::VCM(0x40)); // VCOMH = VREG1OUT * 0.748
			this->_accessMode.writeData(ili9327::VComCmd::VDV(0xf)); // VCOM amplitude=VREG1OUT*1

			// power setting for normal mode

			this->_accessMode.writeCommand(ili9327::PowerSettingNormalModeCmd::Opcode,ili9327::PowerSettingNormalModeCmd::AP(1)); // drivers on
			this->_accessMode.writeData(ili9327::PowerSettingNormalModeCmd::DC(4) | ili9327::PowerSettingNormalModeCmd::DC10(4)); // fosc ratios

			// exit sleep mode

			this->_accessMode.writeCommand(ili9327::ExitSleepModeCmd::Opcode);
			MillisecondTimer::delay(50);

			// enter normal mode

			this->_accessMode.writeCommand(ili9327::EnterNormalModeCmd::Opcode);

			// panel driving setting

			this->_accessMode.writeCommand(ili9327::PanelDrivingCmd::Opcode,0);
			this->_accessMode.writeData(ili9327::PanelDrivingCmd::NL(0x35));
			this->_accessMode.writeData(ili9327::PanelDrivingCmd::SCN(0));
			this->_accessMode.writeData(ili9327::PanelDrivingCmd::PTS(0)); // modify for power reduction in back/front porch
			this->_accessMode.writeData(ili9327::PanelDrivingCmd::ISC(1) | ili9327::PanelDrivingCmd::PTG);
			this->_accessMode.writeData(ili9327::PanelDrivingCmd::DIVE(0));

			// display timing (c1)

			this->_accessMode.writeCommand(ili9327::DisplayTimingSettingNormalModeCmd::Opcode,ili9327::DisplayTimingSettingNormalModeCmd::BC | ili9327::DisplayTimingSettingNormalModeCmd::DIV0_1); // line inversion, 1:1 internal clock
			this->_accessMode.writeData(ili9327::DisplayTimingSettingNormalModeCmd::RTN(0x10)); // 1 line = 16 clocks
			this->_accessMode.writeData(ili9327::DisplayTimingSettingNormalModeCmd::BP(2));
			this->_accessMode.writeData(ili9327::DisplayTimingSettingNormalModeCmd::FP(4));

			// display timing idle (c3)

			this->_accessMode.writeCommand(ili9327::DisplayTimingSettingIdleModeCmd::Opcode,ili9327::DisplayTimingSettingIdleModeCmd::BC2);
			this->_accessMode.writeData(ili9327::DisplayTimingSettingIdleModeCmd::RTN2(0x10));
			this->_accessMode.writeData(ili9327::DisplayTimingSettingIdleModeCmd::BP2(0x2));
			this->_accessMode.writeData(ili9327::DisplayTimingSettingIdleModeCmd::FP2(0x4));

			// frame rate = 96Hz

			this->_accessMode.writeCommand(ili9327::FrameRateAndInversionControlCmd::Opcode,ili9327::FrameRateAndInversionControlCmd::FR_96);

			// interface control

			this->_accessMode.writeCommand(ili9327::InterfaceControlCmd::Opcode,ili9327::InterfaceControlCmd::DPL | ili9327::InterfaceControlCmd::EPL | ili9327::InterfaceControlCmd::HSPL | ili9327::InterfaceControlCmd::VSPL);

			// frame memory access (set DFM for 2 transfers/1 pixel in 18-bit mode)

			this->_accessMode.writeCommand(ili9327::FrameMemoryAccessAndInterfaceSettingCmd::Opcode,0);
			this->_accessMode.writeData(0);
			this->_accessMode.writeData(0);
			this->_accessMode.writeData(ili9327::FrameMemoryAccessAndInterfaceSettingCmd::DFM);

			// display on - cannot happen <120ms after reset

			MillisecondTimer::delay(100);
			this->_accessMode.writeCommand(ili9327::SetDisplayOnCmd::Opcode);

			// set the colour mode (base class specialisation does this)

			this->setColourMode();

			// set the address mode (orientation from specialisation and BGR order)

			this->_accessMode.writeCommand(ili9327::
						SetAddressModeCmd::Opcode,
						this->getOrientationAddressMode() | ili9327::SetAddressModeCmd::BGR | ili9327::SetAddressModeCmd::VERTICAL_FLIP);

			// set scroll area to omit the missing scan lines if panel is shorter than 432

			this->_accessMode.writeCommand(ili9327::SetScrollAreaCmd::Opcode,(432-TPanelTraits::LONG_SIDE) >> 8);
			this->_accessMode.writeData((432-TPanelTraits::LONG_SIDE) & 0xff);
			this->_accessMode.writeData(TPanelTraits::LONG_SIDE >> 8);
			this->_accessMode.writeData(TPanelTraits::LONG_SIDE & 0xff);
			this->_accessMode.writeData(0);
			this->_accessMode.writeData(0);
		}


		/**
		 * Apply the 10 panel gamma settings
		 * @param gamma The collection of gamma values
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(ILI9327Gamma& gamma) const {

			this->_accessMode.writeCommand(ili9327::GammaSettingCmd::Opcode,gamma[0]);

			this->_accessMode.writeData(gamma[1]);
			this->_accessMode.writeData(gamma[2]);
			this->_accessMode.writeData(gamma[3]);
			this->_accessMode.writeData(gamma[4]);
			this->_accessMode.writeData(gamma[5]);
			this->_accessMode.writeData(gamma[6]);
			this->_accessMode.writeData(gamma[7]);
			this->_accessMode.writeData(gamma[8]);
			this->_accessMode.writeData(gamma[9]);
			this->_accessMode.writeData(gamma[10]);
			this->_accessMode.writeData(gamma[11]);
			this->_accessMode.writeData(gamma[12]);
			this->_accessMode.writeData(gamma[13]);
			this->_accessMode.writeData(gamma[14]);
		}


		/**
		 * Send the panel to sleep
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {

			// go to sleep and wait at least 5ms

			this->_accessMode.writeCommand(ili9327::EnterSleepModeCmd::Opcode);
			MillisecondTimer::delay(6);
		}


		/**
		 * Wake the panel up
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {

			// wake up and wait at least 120ms

			this->_accessMode.writeCommand(ili9327::ExitSleepModeCmd::Opcode);
			MillisecondTimer::delay(121);
		}


		/**
		 * Issue the command that allows graphics ram writing to commence
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void ILI9327<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
			this->_accessMode.writeCommand(ili9327::WriteMemoryStartCmd::Opcode);
		}
	}
}
