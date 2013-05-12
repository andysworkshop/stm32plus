/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

	/**
	 * Convenience template to initialise an SPI pin package
	 */

	template<class TPinPackage,class TPeripheralTraits>
	class SpiPinInitialiser {
		public:
			static void initialise(uint16_t mode);
	};


	/**
	 * Initialise the pin package that we were parameterised with
	 */

	template<class TPinPackage,class TPeripheralTraits>
	inline void SpiPinInitialiser<TPinPackage,TPeripheralTraits>::initialise(uint16_t mode) {

		GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SCK,
																	 TPinPackage::Pin_SCK,
																	 Gpio::ALTERNATE_FUNCTION,
																	 (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
																	 Gpio::PUPD_DOWN,
																	 Gpio::PUSH_PULL,
																	 TPeripheralTraits::GPIO_AF);

		if(mode==SPI_Mode_Master) {

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_NSS,
			                               TPinPackage::Pin_NSS,
			                               Gpio::OUTPUT,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_NONE,
			                               Gpio::PUSH_PULL);

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MOSI,
			                               TPinPackage::Pin_MOSI,
			                               Gpio::ALTERNATE_FUNCTION,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_DOWN,
			                               Gpio::PUSH_PULL,
			                               TPeripheralTraits::GPIO_AF);

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MISO,
			                               TPinPackage::Pin_MISO,
			                               Gpio::INPUT,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_DOWN,
			                               Gpio::PUSH_PULL);
		}
		else {

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_NSS,
			                               TPinPackage::Pin_NSS,
			                               Gpio::INPUT,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_NONE,
			                               Gpio::PUSH_PULL);

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MISO,
			                               TPinPackage::Pin_MISO,
			                               Gpio::ALTERNATE_FUNCTION,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_DOWN,
			                               Gpio::PUSH_PULL,
			                               TPeripheralTraits::GPIO_AF);

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MOSI,
			                               TPinPackage::Pin_MOSI,
			                               Gpio::INPUT,
			                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
			                               Gpio::PUPD_DOWN,
			                               Gpio::PUSH_PULL);
		}
	}
}
