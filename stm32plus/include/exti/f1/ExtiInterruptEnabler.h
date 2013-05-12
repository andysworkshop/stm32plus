/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

	/*
	 * Forward declare the IRQ handler names
	 */

	extern "C" void EXTI0_IRQHandler();
	extern "C" void EXTI1_IRQHandler();
	extern "C" void EXTI2_IRQHandler();
	extern "C" void EXTI3_IRQHandler();
	extern "C" void EXTI4_IRQHandler();
	extern "C" void EXTI9_5_IRQHandler();
	extern "C" void EXTI15_10_IRQHandler();
	extern "C" void PVD_IRQHandler();
	extern "C" void RTCAlarm_IRQHandler();
	extern "C" void USBWakeUp_IRQHandler();
	extern "C" void ETH_WKUP_IRQHandler();


	/**
	 * Helper class to enable only the desired interrupts in the NVIC. This will
	 * be fully specialised for each EXTI peripheral
   * @tparam TExtiNumber The number of the Exti peripheral (0..19 | 22)
	 */

	template<uint8_t TExtiNumber>
	class ExtiInterruptEnabler {

		private:
			typedef void (*FPTR)();					// this trick will force the linker to include the ISR
			static FPTR _forceLinkage;

		public:
			static void enable();
	};


  /**
   * Enabler specialisations for F1 and F4
   */

  template<>
  inline void ExtiInterruptEnabler<0>::enable() {
  	_forceLinkage=&EXTI0_IRQHandler;
  	Nvic::configureIrq(EXTI0_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<1>::enable() {
  	_forceLinkage=&EXTI1_IRQHandler;
  	Nvic::configureIrq(EXTI1_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<2>::enable() {
  	_forceLinkage=&EXTI2_IRQHandler;
  	Nvic::configureIrq(EXTI2_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<3>::enable() {
  	_forceLinkage=&EXTI3_IRQHandler;
  	Nvic::configureIrq(EXTI3_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<4>::enable() {
  	_forceLinkage=&EXTI4_IRQHandler;
  	Nvic::configureIrq(EXTI4_IRQn);
  }

  /**
   * 5 through 9 are on a shared IRQ
   */

  template<>
  inline void ExtiInterruptEnabler<5>::enable() {
  	_forceLinkage=&EXTI9_5_IRQHandler;
  	Nvic::configureIrq(EXTI9_5_IRQn);
  }

  /**
   * 10 through 15 are on a shared IRQ
   */

  template<>
  inline void ExtiInterruptEnabler<10>::enable() {
  	_forceLinkage=&EXTI15_10_IRQHandler;
  	Nvic::configureIrq(EXTI15_10_IRQn);
  }

  /**
   * Non-GPIO EXTI lines common to the F1 and F4
   */

  template<>
  inline void ExtiInterruptEnabler<16>::enable() {
  	_forceLinkage=&PVD_IRQHandler;
  	Nvic::configureIrq(PVD_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<17>::enable() {
  	_forceLinkage=&RTCAlarm_IRQHandler;
  	Nvic::configureIrq(RTCAlarm_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<18>::enable() {
  	_forceLinkage=&USBWakeUp_IRQHandler;
  	Nvic::configureIrq(USBWakeUp_IRQn);
  }

  /**
   * Ethernet EXTI is available on the F1/CL and F4
   */

#if defined(STM32PLUS_F1_CL) || defined(STM32PLUS_F4)

  template<>
  inline void ExtiInterruptEnabler<19>::enable() {
  	_forceLinkage=&ETH_WKUP_IRQHandler;
  	Nvic::configureIrq(ETH_WKUP_IRQn);
  }
#endif
}
