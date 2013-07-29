/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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

			this->_accessMode.reset();

			// do the startup sequence

			MillisecondTimer::delay(160);
			_accessMode->writeCommand(hx8352a::TEST_MODE_CONTROL,0x02);        	// TESTM=1
			_accessMode->writeCommand(hx8352a::VDDD_CONTROL,0x02);        			// VDC_SEL=010.
			_accessMode->writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_1,0x00);   // STBA[15:8]=0x00
			_accessMode->writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_2,0xB3);   // STBA[7]=1, STBA[5:4]=11, STBA[1:0]=11
			_accessMode->writeCommand(hx8352a::TEST_MODE_CONTROL,0x00);        	// TESTM=0

			// Power Supply Setting

			_accessMode->writeCommand(hx8352a::OSC_CONTROL_1,0x1);         	 // RADJ=0, OSC_EN=1 (145%)
			_accessMode->writeCommand(hx8352a::DISPLAY_CONTROL_1,0x01);      // TE 0n
			_accessMode->writeCommand(hx8352a::CYCLE_CONTROL_1,0x14);        // N_DCDC=0x14

			MillisecondTimer::delay(20);

			_accessMode->writeCommand(hx8352a::POWER_CONTROL_3,0x14);        // BT=0001, AP=100
			_accessMode->writeCommand(hx8352a::POWER_CONTROL_2,0x11);        // VC3=001, VC1=001 (VLCD/DDVDH)=6.45V)
			_accessMode->writeCommand(hx8352a::POWER_CONTROL_4,0x0E);        // VRH=1110 (VREG1=6.0V)
			_accessMode->writeCommand(hx8352a::POWER_CONTROL_5,0x0F);        // VBGP=1111
			_accessMode->writeCommand(hx8352a::VCOM_CONTROL,0x3B);        	 // VCM=011_1011

			MillisecondTimer::delay(30);

			_accessMode->writeCommand(hx8352a::POWER_CONTROL_1,0x0A);        // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=1, STB=0
			_accessMode->writeCommand(hx8352a::POWER_CONTROL_1,0x1A);        // GASENB=0, PON=1, DK=1, XDK=0, VLCD_TRI=1, STB=0

			MillisecondTimer::delay(50);

			_accessMode->writeCommand(hx8352a::POWER_CONTROL_1,0x12);      	// GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=1, STB=0,

			// VLCD=2XVCI by 2 CAPs

			MillisecondTimer::delay(50);
			_accessMode->writeCommand(hx8352a::POWER_CONTROL_6,0x2E);       // VCOMG=1, VDV=0_1110

			// VCOMG NEW LOCATION

			_accessMode->writeCommand(hx8352a::TEST_MODE_CONTROL,0x02);     // TESTM=1
			_accessMode->writeCommand(0x93,0x10);        										// R93[4]=1, VCOMG=1 ...undocumented!!!
			_accessMode->writeCommand(hx8352a::TEST_MODE_CONTROL,0x00);

			// Display ON Setting
			_accessMode->writeCommand(hx8352a::SOURCE_CONTROL_1,0xFF);      // N_SAP=0111 1111
			_accessMode->writeCommand(hx8352a::SOURCE_CONTROL_2,0x0E);      // I_SAP=0000 1110
			_accessMode->writeCommand(hx8352a::CYCLE_CONTROL_10,0x38);      // EQS=1000 0111
			_accessMode->writeCommand(hx8352a::CYCLE_CONTROL_11,0x38);      // EQP=0011 1000
			_accessMode->writeCommand(hx8352a::DISPLAY_CONTROL_2,0x38);     // GON=1, DTE=1, D=10

			MillisecondTimer::delay(50);
			_accessMode->writeCommand(hx8352a::DISPLAY_CONTROL_2,0x3C);     		// GON=1, DTE=1, D=11
			_accessMode->writeCommand(hx8352a::MEMORY_ACCESS_CONTROL,this->getMemoryAccessControl());
			_accessMode->writeCommand(hx8352a::DISPLAY_MODE_CONTROL,0x02);      // INVON=0, NORNO=1
		}


		/**
		 * Apply the panel gamma settings
		 * @param gamma The gamma class containing the one value
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(HX8352AGamma& gamma) const {
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+0,gamma[0]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+1,gamma[1]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+2,gamma[2]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+3,gamma[3]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+4,gamma[4]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+5,gamma[5]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+6,gamma[6]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+7,gamma[7]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+8,gamma[8]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+9,gamma[9]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+10,gamma[10]);
			this->_accessMode.writeCommand(hx8352a::GAMMA_1+11,gamma[11]);
		}


		/**
		 * Send the panel to sleep
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {
		}


		/**
		 * Wake the panel up
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {
		}


		/**
		 * Issue the command that allows graphics ram writing to commence
		 */

		template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
		inline void HX8352A<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
		  this->_accessMode.writeCommand(hx8352a::MEMORY_WRITE);
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

			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_TOP_FIXED_AREA_H,y >> 8);
			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_TOP_FIXED_AREA_L,y & 0xff);

			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_HEIGHT_H,height >> 8);
			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_HEIGHT_L,height & 0xff);

			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_BOTTOM_FIXED_AREA_H,bfa >> 8);
			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_BOTTOM_FIXED_AREA_L,bfa & 0xff);
		}
	}
}
