/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
	namespace display {
		namespace hx8352a {

			/*
			 * The command set as documented in "version 05 December 2008" of the datasheet
			 * Only the commands we use are defined
			 */

			enum E {
				OSC_CONTROL_1 = 0x17,
				DISPLAY_CONTROL_1 = 0X23,
				MEMORY_WRITE = 0x22,
				CYCLE_CONTROL_1 = 0X2B,
				GAMMA_1 = 0X3E,
				TEST_MODE_CONTROL = 0X83,
				VDDD_CONTROL = 0X85,
				SOURCE_GAMMA_RESISTOR_1 = 0X8B,
				SOURCE_GAMMA_RESISTOR_2 = 0X8C,
				SYNC_FUNCTION = 0X91,
			};
		}
	}
}
