/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

	/**
	 * Base class for I2C peripherals. Takes care of the common operations
	 * that each peripheral can have
	 */

	template<class TPinPackage,PeripheralName TPeripheralName>
	class I2CPeripheral : public I2C {

		protected:
			I2CPeripheral(const Parameters& params);
	};


	/**
	 * Constructor
	 */

	template<class TPinPackage,PeripheralName TPeripheralName>
	inline I2CPeripheral<TPinPackage,TPeripheralName>::I2CPeripheral(const struct Parameters& params)
		: I2C(params,(I2C_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE) {

		I2C_InitTypeDef init;

		// clocks on first before any features get initialised

		ClockControl<TPeripheralName>::On();

		// and next the GPIO pins, also before any features get initialised

		I2CPinInitialiser<TPinPackage,PeripheralTraits<TPeripheralName> >::initialise();

		// initialise the peripheral

		init.I2C_ClockSpeed=params.i2c_clockSpeed;
		init.I2C_Mode=I2C_Mode_I2C;
		init.I2C_DutyCycle=params.i2c_dutyCycle;
		init.I2C_OwnAddress1=params.i2c_ownAddress;
		init.I2C_Ack=params.i2c_ack;
		init.I2C_AcknowledgedAddress=params.i2c_ackAddress;

		I2C_Init(_peripheralAddress,&init);
	}
}
