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

			protected:
				uint32_t _controlBitBandAddress;
				uint32_t _portOutputRegister;
				uint32_t _zero;
				uint32_t _one;
				uint32_t _jump;

			public:
				Gpio16AccessMode();

				void reset() const;

				void writeCommand(uint16_t command) const;
				void writeCommand(uint16_t command,uint16_t parameter) const;
				void writeData(uint16_t value) const;
				void writeDataAgain(uint16_t value) const;
				void writeMultiData(uint32_t howMuch,uint16_t value) const;

				void rawTransfer(const void *buffer,uint32_t numWords) const;
		};


		/**
		 * Constructor
		 */

		template<class TPinPackage>
		inline Gpio16AccessMode<TPinPackage,72,50,50>::Gpio16AccessMode() {

			// These constants are referenced by the assembly parts of this driver. By doing this we can avoid
			// hardcoding registers and therefore let the optimiser have more latitude to do its job.

			_zero=0;
			_one=1;

			// this is the address of the ODR (output data) register for the control port in the bitband region.
			// out here each 32-bit word corresponds to a single bit in the real peripheral register. Therefore
			// we can address the pins by indexing [0..15] from this address.

			_controlBitBandAddress=PERIPH_BB_BASE+((TPinPackage::Port_CONTROL-PERIPH_BASE+offsetof(GPIO_TypeDef,ODR))*32);

			// this is the address of the data output ODR register in the normal peripheral region.

			_portOutputRegister=TPinPackage::Port_DATA+offsetof(GPIO_TypeDef,ODR);

			// all 16 port pins to output, 50MHz slew rate

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_DATA,
			                               0xffff,
			                               Gpio::OUTPUT);

			// control pins to output

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_CONTROL,
			                               ((1 << TPinPackage::Pin_RS) |
			                              	(1 << TPinPackage::Pin_WR) |
			                              	(1 << TPinPackage::Pin_RESET)),
			                               Gpio::OUTPUT);

			// WR must start as HIGH

			GPIO_SetBits((GPIO_TypeDef *)TPinPackage::Port_CONTROL,1 << TPinPackage::Pin_WR);
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

			port=(GPIO_TypeDef *)TPinPackage::Port_CONTROL;
			pin=1 << TPinPackage::Pin_RESET;

			GPIO_SetBits(port,pin);
			MillisecondTimer::delay(5);
			GPIO_ResetBits(port,pin);
			MillisecondTimer::delay(50);
			GPIO_SetBits(port,pin);
			MillisecondTimer::delay(50);
		}


		/**
		 * Write a command
		 * @param command The command register
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeCommand(uint16_t command) const {

			__asm volatile(
				" str  %[value], [%[data]]              	\n\t"			// port <= value
				" str  %[zero],  [%[commands], %[rs]]    	\n\t"     // [rs] = 0
				" str  %[zero],  [%[commands], %[wr]]    	\n\t"			// [wr] = 0
				" str  %[one],   [%[commands], %[wr]]    	\n\t"     // [wr] = 1
				:: [commands] "r" (_controlBitBandAddress),					// the control address
				   [data]     "r" (_portOutputRegister),						// the data port
				   [wr]       "I" (TPinPackage::Pin_WR * 4),				// 4 bytes per bit-band bit (base is ODR bit 0)
				   [rs]       "I" (TPinPackage::Pin_RS * 4),				// ditto
				   [value]    "r" (command),												// input value
				   [zero]     "r" (_zero),
				   [one]      "r" (_one)
			);
		}


		/**
		 * Write a command and its parameter (convenience function)
		 * @param command The command register
		 * @param parameter The register parameter
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeCommand(uint16_t command,uint16_t parameter) const {
			writeCommand(command);
			writeData(parameter);
		}


		/**
		 * Write a data value
		 * @param value The data value to write
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeData(uint16_t value) const {

			__asm volatile(
				" str  %[value], [%[data]]              	\n\t"			// port <= value
				" str  %[one],   [%[commands], %[rs]]    	\n\t"     // [rs] = 1
				" str  %[zero],  [%[commands], %[wr]]    	\n\t"			// [wr] = 0
				" str  %[one],   [%[commands], %[wr]]    	\n\t"     // [wr] = 1
				:: [commands] "r" (_controlBitBandAddress),					// the control address
				   [data]     "r" (_portOutputRegister),						// the data port
				   [wr]       "I" (TPinPackage::Pin_WR * 4),				// 4 bytes per bit-band bit (base is ODR bit 0)
				   [rs]       "I" (TPinPackage::Pin_RS * 4),				// ditto
				   [value]    "r" (value),													// input value
				   [zero]     "r" (_zero),
				   [one]      "r" (_one)
			);
		}


		/**
		 * Write the same data value that we have recently written out. This is one of our optimisation
		 * points. We don't have to do the whole 8080 transaction again and can just toggle WR.
		 * @param value The data value to write
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeDataAgain(uint16_t /* value */) const {

			__asm volatile(
				" str  %[zero],  [%[commands], %[wr]]    	\n\t"			// [wr] = 0
				" str  %[one],   [%[commands], %[wr]]    	\n\t"     // [wr] = 1
				:: [commands] "r" (_controlBitBandAddress),					// the control address
				   [wr]       "I" (TPinPackage::Pin_WR * 4),				// 4 bytes per bit-band bit (base is ODR bit 0)
				   [zero]     "r" (_zero),
				   [one]      "r" (_one)
			);
		}

			/**
	 * Write a batch of the same data values to the XMEM interface using GPIO. The values are written out in a
	 * highly optimised loop in bursts of 40 at a time. This value seems a good trade off between flash usage
	 * and speed. The turnaround time between batches has been measured at around 1 microsecond. Note the use
	 * of %= labels so that inlining doesn't produce duplicate names.
	 * @param howMuch The number of 16-bit values to write
	 * @param lo8 The low 8 bits of the value to write
	 * @param hi8 The high 8 bits of the value to write. Many parameter values are 8-bits so this parameters defaults to zero.
	 */

	template<class TPinPackage>
	inline void Gpio16AccessMode<TPinPackage,72,50,50>::writeMultiData(uint32_t howMuch,uint16_t value) const {

		__asm volatile(
				"    str  %[value],   [%[data]]             		\n\t"			// port <= value
				"    str  %[one],     [%[commands], %[rs]]  		\n\t"     // [rs] = 1
				"    cmp  %[howmuch], #40                       \n\t"
				"    blo  lastlot%=                             \n\t"

				"batchloop%=:                               		\n\t"
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1

				"    sub  %[howmuch], #40                       \n\t"     // subtract 40 from howMuch
				"    cmp  %[howmuch], #40 											\n\t"     // if howMuch >= 40 then go back for another batch
				"    bhs  batchloop%=                           \n\t"

				"lastlot%=:                                     \n\t"

				"    ldr %[jump],    =finished%=                \n\t"			// load jump with the address of the end
				"    lsl %[howmuch], #2                         \n\t"			// multiply remaining by 4 and
				"    sub %[jump],    %[howmuch]                 \n\t"			// subtract from the jump target
				"    orr %[jump],    #1                         \n\t"			// set thumb mode (bit 0=1)
				"    bx  %[jump]                                \n\t"			// indirect jump into the last lot

				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1
				"    str  %[zero],  [%[commands], %[wr]]  			\n\t"			// [wr] = 0
				"    str  %[one],   [%[commands], %[wr]]    	  \n\t"     // [wr] = 1

				"finished%=:           \n\t"

				:: [commands] "r" (_controlBitBandAddress),					// the control address
				   [data]     "r" (_portOutputRegister),						// the data port
				   [wr]       "I" (TPinPackage::Pin_WR * 4),				// 4 bytes per bit-band bit (base is ODR bit 0)
				   [rs]       "I" (TPinPackage::Pin_RS * 4),				// ditto
				   [value]    "r" (value),													// input value
				   [zero]     "r" (_zero),
				   [one]      "r" (_one),
				   [jump]     "r" (_jump),
				   [howmuch]  "r" (howMuch)
		);
	}


		/**
		 * Write out a raw block of data from memory
		 * @param buffer Where to read from
		 * @param numWords The number of 16-bit words
		 */

		template<class TPinPackage>
		inline void Gpio16AccessMode<TPinPackage,72,50,50>::rawTransfer(const void *buffer,uint32_t numWords) const {

			const uint16_t *ptr=static_cast<const uint16_t *>(buffer);

			while(numWords--)
				writeData(*ptr++);
		}
	}
}
