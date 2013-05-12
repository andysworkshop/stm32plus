/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fsmc.h"


namespace stm32plus {

	/**
	 * Default constructor, zero out the typedef
	 */

	FsmcAddressDataSetupTiming::FsmcAddressDataSetupTiming() :
		FSMC_NORSRAMTimingInitTypeDef() {
	}

	/**
	 * Constructor with address and data setup. The most likely two parameters that you will need to set.
	 *
	 * @param[in] addressSetup Address setup HCLK cycles.
	 * @param[in] dataSetup Data setup HCLK cycles.
	 */

	FsmcAddressDataSetupTiming::FsmcAddressDataSetupTiming(uint32_t addressSetup,uint32_t dataSetup) :
		FSMC_NORSRAMTimingInitTypeDef() {

		FSMC_AddressSetupTime=addressSetup;
		FSMC_DataSetupTime=dataSetup;
	}

}
