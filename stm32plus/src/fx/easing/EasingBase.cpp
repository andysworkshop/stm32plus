/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
	namespace fx {

		/**
		 * Default constructor
		 */

		EasingBase::EasingBase() {
			_change=0;
		}

		/**
		 * Set the duration
		 * @param[in] duration_ The duration
		 */

		void EasingBase::setDuration(double duration_) {
			_duration=duration_;
		}

		/**
		 * Set the total change in position
		 * @param[in] totalChangeInPosition_ The total change in position.
		 */

		void EasingBase::setTotalChangeInPosition(double totalChangeInPosition_) {
			_change=totalChangeInPosition_;
		}
	}
}
