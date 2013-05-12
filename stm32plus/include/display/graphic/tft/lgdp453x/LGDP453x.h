/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/lgdp453x/LGDP453xColour.h"
#include "display/graphic/tft/lgdp453x/LGDP453xOrientation.h"
#include "display/graphic/tft/lgdp453x/LGDP453xGamma.h"
#include "display/graphic/tft/lgdp453x/commands/AllCommands.h"


namespace stm32plus {
	namespace display {

		/**
		 * Generic LGDP453x template. The user can specialise based on the desired colour
		 * depth, orientation and access mode.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		class LGDP453x : public LGDP453xColour<TColourDepth,TAccessMode>,
				public LGDP453xOrientation<TOrientation,TAccessMode> {

			public:
				enum {
					SHORT_SIDE = 240,
					LONG_SIDE = 320
				};

			protected:
				TAccessMode& _accessMode;

			public:
				LGDP453x(TAccessMode& accessMode);

				void initialise() const;

				void applyGamma(LGDP453xGamma& gamma) const;
				void sleep() const;
				void wake() const;
				void beginWriting() const;
		};


		/**
		 * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
		 * common base class for use by all.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline LGDP453x<TOrientation,TColourDepth,TAccessMode>::LGDP453x(TAccessMode& accessMode)
			: LGDP453xColour<TColourDepth,TAccessMode>(accessMode),
			  LGDP453xOrientation<TOrientation,TAccessMode>(accessMode),
			  _accessMode(accessMode) {
		}


		/**
		 * Initialise the LCD. Do the reset sequence.
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void LGDP453x<TOrientation,TColourDepth,TAccessMode>::initialise() const {

			// reset the device
			this->_accessMode.reset();
			// start oscillator
			this->_accessMode.writeCommand(lgdp453x::StartOscillationCmd::Opcode,lgdp453x::StartOscillationCmd::Enable);
			MillisecondTimer::delay(10);

			// regulator control
			//130131-andy this->_accessMode.writeCommand(lgdp453x::RegulatorControlCmd::Opcode,0x0030);
			this->_accessMode.writeCommand(lgdp453x::RegulatorControlCmd::Opcode,lgdp453x::RegulatorControlCmd::RV(3));

			// power On sequence
			// this->_accessMode.writeCommand(0x0011,0x0040);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl2Cmd::Opcode,lgdp453x::PowerCtrl2Cmd::DC0(4));

			// this->_accessMode.writeCommand(0x0010,0x1628);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl1Cmd::Opcode,lgdp453x::PowerCtrl1Cmd::SAP(1)|lgdp453x::PowerCtrl1Cmd::BT(6)|lgdp453x::PowerCtrl1Cmd::AP(2)|lgdp453x::PowerCtrl1Cmd::DK);

			// this->_accessMode.writeCommand(0x0012,0x0000);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl3Cmd::Opcode,0);

			// this->_accessMode.writeCommand(0x0013,0x104d);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl4Cmd::Opcode,lgdp453x::PowerCtrl4Cmd::VDV(16)|lgdp453x::PowerCtrl4Cmd::VCM(0x4d));

			MillisecondTimer::delay(10);

			// this->_accessMode.writeCommand(0x0012,0x0010);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl3Cmd::Opcode,lgdp453x::PowerCtrl3Cmd::PON);

			MillisecondTimer::delay(10);

			// this->_accessMode.writeCommand(0x0010,0x2620);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl1Cmd::Opcode,lgdp453x::PowerCtrl1Cmd::SAP(2)|lgdp453x::PowerCtrl1Cmd::BT(6)|lgdp453x::PowerCtrl1Cmd::AP(2));

			// this->_accessMode.writeCommand(0x0013,0x344d);
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl4Cmd::Opcode,lgdp453x::PowerCtrl4Cmd::VCOMG|lgdp453x::PowerCtrl4Cmd::VDV(0x18)|lgdp453x::PowerCtrl4Cmd::VCM(0x4d));
			MillisecondTimer::delay(10);

			// this->_accessMode.writeCommand(0x0001,0x0100);
			this->_accessMode.writeCommand(lgdp453x::DriverOutputControlCmd::Opcode,lgdp453x::DriverOutputControlCmd::SS);

			// this->_accessMode.writeCommand(0x0002,0x0300);
			this->_accessMode.writeCommand(lgdp453x::DrivingWaveControlCmd::Opcode,lgdp453x::DrivingWaveControlCmd::EOR | lgdp453x::DrivingWaveControlCmd::BC0);

			// this->_accessMode.writeCommand(0x0003,0x1030);
			this->_accessMode.writeCommand(lgdp453x::EntryModeCmd::Opcode,this->getColourEntryMode() | this->getOrientationEntryMode() | lgdp453x::EntryModeCmd::BGR);

			// this->_accessMode.writeCommand(0x0008,0x0604);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl2Cmd::Opcode,lgdp453x::DisplayCtrl2Cmd::FP(6)|lgdp453x::DisplayCtrl2Cmd::BP(4));

			// this->_accessMode.writeCommand(0x0009,0x0000);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl3Cmd::Opcode,0);

			// this->_accessMode.writeCommand(0x000A,0x0008);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl4Cmd::Opcode,lgdp453x::DisplayCtrl4Cmd::FMARKOE);

			// this->_accessMode.writeCommand(0x0041,0x0002);
			this->_accessMode.writeCommand(lgdp453x::EpromControlRegister2::Opcode,lgdp453x::EpromControlRegister2::VCMSEL(2));

			// this->_accessMode.writeCommand(0x0060,0x2700);
			this->_accessMode.writeCommand(lgdp453x::GateScanControl1Cmd::Opcode,lgdp453x::GateScanControl1Cmd::NL(0x27));

			// this->_accessMode.writeCommand(0x0061,0x0001);
			this->_accessMode.writeCommand(lgdp453x::GateScanControl2Cmd::Opcode,lgdp453x::GateScanControl2Cmd::REV | lgdp453x::GateScanControl2Cmd::VLE);

			// this->_accessMode.writeCommand(0x0090,0x0182); // ???
			this->_accessMode.writeCommand(lgdp453x::PanelInterfaceControl1::Opcode,lgdp453x::PanelInterfaceControl1::DIVI(1)|lgdp453x::PanelInterfaceControl1::RTNI(0x41));

			// this->_accessMode.writeCommand(0x0093,0x0001);
			this->_accessMode.writeCommand(lgdp453x::PanelInterfaceControl3::Opcode,lgdp453x::PanelInterfaceControl3::MCPI(1));

			// this->_accessMode.writeCommand(0x00a3,0x0010);
			this->_accessMode.writeCommand(lgdp453x::TestRegister4::Opcode,lgdp453x::TestRegister4::RDSM(1));
			MillisecondTimer::delay(10);
			
			// display ON
			// this->_accessMode.writeCommand(0x0007,0x0001);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::D(1));


			MillisecondTimer::delay(10);
//*****************************************************************
			// this->_accessMode.writeCommand(0x0007,0x0021);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::GON | lgdp453x::DisplayCtrl1Cmd::D(1));
			// this->_accessMode.writeCommand(0x0007,0x0023);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::GON | lgdp453x::DisplayCtrl1Cmd::D(3));
			MillisecondTimer::delay(10);
			// this->_accessMode.writeCommand(0x0007,0x0033);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::GON | lgdp453x::DisplayCtrl1Cmd::DTE | lgdp453x::DisplayCtrl1Cmd::D(3));
			MillisecondTimer::delay(10);
			// this->_accessMode.writeCommand(0x0007,0x0133);
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::BASEE | lgdp453x::DisplayCtrl1Cmd::GON | lgdp453x::DisplayCtrl1Cmd::DTE | lgdp453x::DisplayCtrl1Cmd::D(3));
		}


		/**
		 * Apply the 10 panel gamma settings
		 * @param gamma The collection of gamma values
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void LGDP453x<TOrientation,TColourDepth,TAccessMode>::applyGamma(LGDP453xGamma& gamma) const {

			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+0,gamma[0]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+1,gamma[1]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+2,gamma[2]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+3,gamma[3]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+4,gamma[4]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+5,gamma[5]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+6,gamma[6]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+7,gamma[7]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+8,gamma[8]);
			this->_accessMode.writeCommand(lgdp453x::GammaControlCmd::FirstOpcode+9,gamma[9]);
		}


		/**
		 * Send the panel to sleep
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void LGDP453x<TOrientation,TColourDepth,TAccessMode>::sleep() const {
		  this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::D(0));
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl1Cmd::Opcode,lgdp453x::PowerCtrl1Cmd::SLP);
		}


		/**
		 * Wake the panel up
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void LGDP453x<TOrientation,TColourDepth,TAccessMode>::wake() const {
			this->_accessMode.writeCommand(lgdp453x::PowerCtrl1Cmd::Opcode,lgdp453x::PowerCtrl1Cmd::AP(7) | lgdp453x::PowerCtrl1Cmd::BT(6) | lgdp453x::PowerCtrl1Cmd::SAP(1));
			this->_accessMode.writeCommand(lgdp453x::DisplayCtrl1Cmd::Opcode,lgdp453x::DisplayCtrl1Cmd::D(3) | lgdp453x::DisplayCtrl1Cmd::GON  | lgdp453x::DisplayCtrl1Cmd::DTE | lgdp453x::DisplayCtrl1Cmd::BASEE);
		}


		/**
		 * Issue the command that allows graphics ram writing to commence
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode>
		inline void LGDP453x<TOrientation,TColourDepth,TAccessMode>::beginWriting() const {
		  this->_accessMode.writeCommand(lgdp453x::GRAMStartWritingCmd::Opcode);
		}
	}
}
