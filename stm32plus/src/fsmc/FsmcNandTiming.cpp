/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fsmc.h"


namespace stm32plus {

	/**
	 * Constructor - setup the timing parameters. See FSMC_NAND_PCCARDTimingInitTypeDef for
	 * the timing parameter explanation.
	 * @param setupTime
	 * @param waitSetupTime
	 * @param holdSetupTime
	 * @param hizSetupTime
	 */

	FsmcNandTiming::FsmcNandTiming(
			uint32_t setupTime,
			uint32_t waitSetupTime,
			uint32_t holdSetupTime,
			uint32_t hizSetupTime) {

		// setup the parameters

		FSMC_SetupTime=setupTime;
		FSMC_WaitSetupTime=waitSetupTime;
		FSMC_HoldSetupTime=holdSetupTime;
		FSMC_HiZSetupTime=hizSetupTime;
	}
}
