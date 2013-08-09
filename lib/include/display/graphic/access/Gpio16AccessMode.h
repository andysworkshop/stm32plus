/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace display {


		/**
		 * Forward declaration for the template specialisations
		 */

		template<class TPinPackage,uint16_t TClockFrequency,uint16_t TLow,uint16_t THigh>
		class Gpio16AccessMode;


		/**
		 * Specialisation for 72Mhz MCU clock, 50ns low and 50ns high write cycle periods
		 */

		template<class TPinPackage>
		class Gpio16AccessMode<TPinPackage,72,50,50> {

			public:
				Gpio16AccessMode();

				void reset() const;

				void writeCommand(uint16_t command) const;
				void writeCommand(uint16_t command,uint16_t parameter) const;
				void writeData(uint16_t value) const;
				void writeDataAgain(uint16_t value) const;

				void rawTransfer(const void *buffer,uint32_t numWords) const;
		};


		/**
		 * Constructor
		 */

		template<class TPinPackage>
		inline Gpio16AccessMode<TPinPackage,72,50,50>::Gpio16AccessMode() {

			// all 16 port pins to output, 50MHz slew rate

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_DATA,
			                               0xffff,
			                               Gpio::OUTPUT);

			// control pins to output

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_COMMANDS,
			                               ((1 << TPinPackage::Pin_RS) |
			                              	(1 << TPinPackage::Pin_WR) |
			                              	(1 << TPinPackage::Pin_RESET)),
			                               Gpio::OUTPUT);

			// WR must start as HIGH

			GPIO_SetBits((GPIO_TypeDef *)TPinPackage::Port_COMMANDS,1 << TPinPackage::Pin_WR);
		}


		/**
		 * Hard-reset the panel
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::reset() const {

			GPIO_TypeDef *port;
			uint16_t pin;

			// let the power stabilise

			MillisecondTimer::delay(10);

			// reset sequence

			port=(GPIO_TypeDef *)TPinPackage::Port_COMMANDS;
			pin=1 << TPinPackage::Pin_RESET;

			GPIO_SetBits(port,pin);
			MillisecondTimer::delay(5);
			GPIO_ResetBits(port,pin);
			MillisecondTimer::delay(50);
			GPIO_SetBits(port,pin);
			MillisecondTimer::delay(50);
		}


		/**
		 * write a command
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeCommand(uint16_t /* command */) const {
		}


		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeCommand(uint16_t /* command */,uint16_t /* parameter */) const {
		}


		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeData(uint16_t value) const {

			// all these instructions are single cycle = 13.8ns

			__asm volatile(
				" movw r4,       #0              \n\t"     	// r4 = 0
				" ldr  r5,       =%[commands]    \n\t"			// r5 = bit band base for command port
				" ldr  r6,       =%[data]        \n\t"			// r6 = ordinary port base for data
				" str  r4,       [r5, %[wr]]    \n\t"			// [wr] = 0
				" movw r4,       #1              \n\t"			// r4 = 1
				" str  %[value], r6              \n\t"			// port <= value
				" str  r4,       [r5, %[rs]]    \n\t"      // [rs] = 1
				" str  r4,       [r5, %[wr]]    \n\t"      // [wr] = 1

				:: [commands] "X" (PERIPH_BB_BASE+((TPinPackage::Port_COMMANDS-PERIPH_BASE+offsetof(GPIO_TypeDef,ODR))*32)),
				   [data]     "X" (TPinPackage::Port_DATA+offsetof(GPIO_TypeDef,ODR)),
				   [wr]       "I" (TPinPackage::Pin_WR * 4),			// 4 bytes per bit-band bit (base is ODR bit 0)
				   [rs]       "I" (TPinPackage::Pin_RS * 4),			// ditto
				   [value]    "r" (value)													// input value
				: "r4","r5","r6"
			);
		}


		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeDataAgain(uint16_t value) const {
			writeData(value);
		}


		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::rawTransfer(const void * /* buffer */,uint32_t /* numWords */) const {
		}
	}
}
