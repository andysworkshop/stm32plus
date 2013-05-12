/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


	class Gpio;

	/**
	 * Base class for the GpioPort template class
	 */

	class GpioPortBase {

		public:
			Gpio *_pinHandlers[16];
			GPIO_TypeDef *_peripheralAddress;

		public:
			GpioPortBase(GPIO_TypeDef *peripheralAddress);

			void setPinHandler(uint8_t index,Gpio *pinHandler);
			GPIO_TypeDef * getPeripheralAddress() const;
	};


	/**
	 * Constructor
	 * @param peripheralAddress
	 */

	inline GpioPortBase::GpioPortBase(GPIO_TypeDef *peripheralAddress)
		: _peripheralAddress(peripheralAddress) {
	}


	/**
	 * Set the pin handler for the pin at a given position
	 * @param index The position (0..15)
	 * @param pinHandler The handler for that position
	 */

	inline void GpioPortBase::setPinHandler(uint8_t index,Gpio *pinHandler) {
		_pinHandlers[index]=pinHandler;
	}


	/**
	 * Cast to the GPIO peripheral address
	 */

	inline GPIO_TypeDef *GpioPortBase::getPeripheralAddress() const {
		return _peripheralAddress;
	}
}
