/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
	namespace fx {

		/*
		 * Ease in
		 */

		double LinearEase::easeIn(double time_) const {
			return _change * time_ / _duration;
		}

		/*
		 * Ease out
		 */

		double LinearEase::easeOut(double time_) const {
			return easeIn(time_);
		}

		/*
		 * Ease in and out
		 */

		double LinearEase::easeInOut(double time_) const {
			return easeIn(time_);
		}
	}
}
