/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {

	/**
	 * Specialisation of the AF mapper for this peripheral. The F4 has the same AF ID
	 * regardless of the port so we specialise only on the peripheral
	 */

	template<uint32_t TPort,uint16_t TPin>
	struct GpioAlternateFunctionMapper<PERIPHERAL_I2C1,TPort,TPin> {
		enum {
			GPIO_AF = GPIO_AF_I2C1
		};
	};

	template<uint32_t TPort,uint16_t TPin>
	struct GpioAlternateFunctionMapper<PERIPHERAL_I2C2,TPort,TPin> {
		enum {
			GPIO_AF = GPIO_AF_I2C2
		};
	};

	template<uint32_t TPort,uint16_t TPin>
	struct GpioAlternateFunctionMapper<PERIPHERAL_I2C3,TPort,TPin> {
		enum {
			GPIO_AF = GPIO_AF_I2C3
		};
	};
}
