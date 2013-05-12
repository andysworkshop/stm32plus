/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


	/**
	 * Convenience template to initialise an I2C pin package
	 */

	template<class TPinPackage,class TPeripheralTraits>
	class I2CPinInitialiser {
		public:
			static void initialise();
	};


	/**
	 * Initialise the pin package that we were parameterised with. The pin modes are the
	 * same on the F1 and F4 - both are AF-OD.
	 */

	template<class TPinPackage,class TPeripheralTraits>
	inline void I2CPinInitialiser<TPinPackage,TPeripheralTraits>::initialise() {

		GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SCL,
		                               TPinPackage::Pin_SCL,
		                               Gpio::ALTERNATE_FUNCTION,
		                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
		                               Gpio::PUPD_NONE,
		                               Gpio::OPEN_DRAIN,
		                               TPeripheralTraits::GPIO_AF);

		GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SDA,
		                               TPinPackage::Pin_SDA,
		                               Gpio::ALTERNATE_FUNCTION,
		                               (GPIOSpeed_TypeDef)TPeripheralTraits::GPIO_SPEED,
		                               Gpio::PUPD_NONE,
		                               Gpio::OPEN_DRAIN,
		                               TPeripheralTraits::GPIO_AF);
	}
}
