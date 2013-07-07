/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {


	/**
	 * This is the dummy implementation for the F1.
	 */

	template<PeripheralName TPeripheralName,uint32_t TPort,uint16_t TPin>
	struct GpioAlternateFunctionMapper {
		enum {
			GPIO_AF = 0				///< never used
		};
	};
}
