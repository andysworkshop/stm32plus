/*
 * CanPiniInitialiser.h
 *
 *  Created on: 2015. nov. 10.
 *      Author: elektroman
 */


#pragma once

namespace stm32plus {
/**
   * Convenience template to initialise a CAN pin package
   */
template<class TPinPackage,PeripheralName>
class CanPinInitialiser {
public:
	static void initialise();

};
/**
   * Initialise the pin package that we were parameterised with
   */
template<class TPinPackage,PeripheralName TPeripheralName>
inline void CanPinInitialiser<TPinPackage,TPeripheralName>::initialise(){
	GpioPinInitialiser::initialise(
			(GPIO_TypeDef *)TPinPackage::Port_TX,
			TPinPackage::Pin_TX,
			Gpio::ALTERNATE_FUNCTION,
            (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
            Gpio::PUPD_NONE,
            Gpio::PUSH_PULL,
            GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_TX,TPinPackage::Pin_TX>::GPIO_AF);

	GpioPinInitialiser::initialise(
	            (GPIO_TypeDef *)TPinPackage::Port_RX,
	            TPinPackage::Pin_RX,
				Gpio::INPUT,
	            (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
	            Gpio::PUPD_UP,
				GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_RX,TPinPackage::Pin_RX>::GPIO_AF);
	}


}  // namespace stm32plus
