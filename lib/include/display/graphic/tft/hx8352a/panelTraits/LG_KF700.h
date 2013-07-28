/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
	namespace display {


		/**
		 * Panel traits for the LG KF700 display. The panel is 480x240 pixels in size.
		 */

		class LG_KF700 {
			public:
				enum {
					LONG_SIDE = 480,
					SHORT_SIDE = 240
				};
		};
	}
}
