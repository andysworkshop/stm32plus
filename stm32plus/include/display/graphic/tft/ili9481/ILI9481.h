/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/ili9481/ILI9481Colour.h"
#include "display/graphic/tft/ili9481/ILI9481Orientation.h"
#include "display/graphic/tft/ili9481/ILI9481Gamma.h"
#include "display/graphic/tft/ili9481/commands/AllCommands.h"


namespace stm32plus {
	namespace display {

		/**
		 * Generic ILI9481 template. The user can specialise based on the desired colour
		 * depth, orientation and access mode.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		class ILI9481 : public ILI9481Colour<TColourDepth,TAccessMode>,
										public ILI9481Orientation<TOrientation,TAccessMode> {

			public:
				enum {
					SHORT_SIDE = 320,
					LONG_SIDE = 480
				};

			protected:
				TAccessMode& _accessMode;

			public:
				ILI9481(TAccessMode& accessMode);

				void initialise() const;

				void applyGamma(ILI9481Gamma& gamma) const;
				void sleep() const;
				void wake() const;
				void beginWriting() const;
		};


		/**
		 * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
		 * common base class for use by all.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline ILI9481<TOrientation,TColourDepth,TAccessMode>::ILI9481(TAccessMode& accessMode)
			: ILI9481Colour<TColourDepth,TAccessMode>(accessMode),
			  ILI9481Orientation<TOrientation,TAccessMode>(accessMode),
		  _accessMode(accessMode) {
		}


		/**
		 * Initialise the LCD. Do the reset sequence.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void ILI9481<TOrientation,TColourDepth,TAccessMode>::initialise() const {

			// reset the device

			this->_accessMode.reset();

			// exit sleep mode

			this->_accessMode.writeCommand(ili9481::ExitSleepModeCmd::Opcode);
			MillisecondTimer::delay(50);

			// enter normal mode

			this->_accessMode.writeCommand(ili9481::EnterNormalModeCmd::Opcode);

			// power setting

			this->_accessMode.writeCommand(ili9481::PowerSettingCmd::Opcode,ili9481::PowerSettingCmd::VC_100);
			this->_accessMode.writeData(ili9481::PowerSettingCmd::PON | ili9481::PowerSettingCmd::BT_5_5);
			this->_accessMode.writeData(ili9481::PowerSettingCmd::VRH(5) | ili9481::PowerSettingCmd::VCIRE);

			// VCom (more power settings)

			this->_accessMode.writeCommand(ili9481::VComCmd::Opcode,0); // register D1 for setting VCom
			this->_accessMode.writeData(ili9481::VComCmd::VCM(0));
			this->_accessMode.writeData(ili9481::VComCmd::VDV(0));

			// power setting for normal mode

			this->_accessMode.writeCommand(ili9481::PowerSettingNormalModeCmd::Opcode,ili9481::PowerSettingNormalModeCmd::AP(1)); // drivers on
			this->_accessMode.writeData(ili9481::PowerSettingNormalModeCmd::DC(2) | ili9481::PowerSettingNormalModeCmd::DC10(0)); // fosc ratios

			// panel driving setting

			this->_accessMode.writeCommand(ili9481::PanelDrivingCmd::Opcode,0);
			this->_accessMode.writeData(ili9481::PanelDrivingCmd::NL(0x3b));
			this->_accessMode.writeData(ili9481::PanelDrivingCmd::SCN(0));
			this->_accessMode.writeData(2);
			this->_accessMode.writeData(ili9481::PanelDrivingCmd::ISC(1) | ili9481::PanelDrivingCmd::PTG);

			// display timing (c1)

			this->_accessMode.writeCommand(ili9481::DisplayTimingSettingNormalModeCmd::Opcode,ili9481::DisplayTimingSettingNormalModeCmd::BC | ili9481::DisplayTimingSettingNormalModeCmd::DIV0_1); // line inversion, 1:1 internal clock
			this->_accessMode.writeData(ili9481::DisplayTimingSettingNormalModeCmd::RTN(16)); // 1 line = 16 clocks
			this->_accessMode.writeData(ili9481::DisplayTimingSettingNormalModeCmd::FP(8) | ili9481::DisplayTimingSettingNormalModeCmd::BP(8));

			// display timing idle (c3)

			this->_accessMode.writeCommand(ili9481::DisplayTimingSettingIdleModeCmd::Opcode,ili9481::DisplayTimingSettingIdleModeCmd::BC2);
			this->_accessMode.writeData(ili9481::DisplayTimingSettingIdleModeCmd::RTN2(0x20));
			this->_accessMode.writeData(ili9481::DisplayTimingSettingIdleModeCmd::BP2(0x8));

			// frame rate = 85Hz

			this->_accessMode.writeCommand(ili9481::FrameRateAndInversionControlCmd::Opcode,ili9481::FrameRateAndInversionControlCmd::FR_72);

			// interface control

			this->_accessMode.writeCommand(ili9481::InterfaceControlCmd::Opcode,ili9481::InterfaceControlCmd::DPL | ili9481::InterfaceControlCmd::EPL | ili9481::InterfaceControlCmd::HSPL | ili9481::InterfaceControlCmd::VSPL);

			// frame memory access (set DFM for 2 transfers/1 pixel in 18-bit mode)

			this->_accessMode.writeCommand(ili9481::FrameMemoryAccessAndInterfaceSettingCmd::Opcode,0);
			this->_accessMode.writeData(0);
			this->_accessMode.writeData(0);
			this->_accessMode.writeData(ili9481::FrameMemoryAccessAndInterfaceSettingCmd::DFM);

			// set the colour depth and orientation

			this->_accessMode.writeCommand(ili9481::SetPixelFormatCmd::Opcode,this->getPixelFormat());
			this->_accessMode.writeCommand(ili9481::SetAddressModeCmd::Opcode,this->getAddressMode() | ili9481::SetAddressModeCmd::BGR);

			// display on

			MillisecondTimer::delay(100);
			this->_accessMode.writeCommand(ili9481::SetDisplayOnCmd::Opcode);
		}


		/**
		 * Apply the 11 panel gamma settings
		 * @param gamma The collection of gamma values
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void ILI9481<TOrientation,TColourDepth,TAccessMode>::applyGamma(ILI9481Gamma& gamma) const {

			this->_accessMode.writeCommand(ili9481::GammaSettingCmd::Opcode,gamma[0]);

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
		}


		/**
		 * Send the panel to sleep
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void ILI9481<TOrientation,TColourDepth,TAccessMode>::sleep() const {
			this->_accessMode.writeCommand(ili9481::EnterSleepModeCmd::Opcode);
			MillisecondTimer::delay(120);
		}


		/**
		 * Wake the panel up
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void ILI9481<TOrientation,TColourDepth,TAccessMode>::wake() const {
			this->_accessMode.writeCommand(ili9481::ExitSleepModeCmd::Opcode);
			MillisecondTimer::delay(120);
		}


		/**
		 * Issue the command that allows graphics ram writing to commence
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void ILI9481<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
		  this->_accessMode.writeCommand(ili9481::WriteMemoryStartCmd::Opcode);
		}
	}
}
