/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

	/**
	 * @brief Timing class for devices that just need address and data setup times
	 *
	 * Inherits from FSMC_NORSRAMTimingInitTypeDef to allow
	 * the user to customise it.
	 */

	class FsmcAddressDataSetupTiming : public FSMC_NORSRAMTimingInitTypeDef {
		public:
			FsmcAddressDataSetupTiming();
			FsmcAddressDataSetupTiming(uint32_t addressSetup,uint32_t dataSetup);
	};
}
