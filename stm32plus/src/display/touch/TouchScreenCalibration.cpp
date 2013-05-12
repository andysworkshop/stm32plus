/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/touch.h"


namespace stm32plus {
	namespace display {

		/*
		 * Translate: Do nothing, just copy
		 */

		Point PassThroughTouchScreenCalibration::translate(const Point& rawPoint) {
			return Point(rawPoint);
		}


		/*
		 * Can't serialise
		 */

		bool PassThroughTouchScreenCalibration::serialise(OutputStream& ostream __attribute__((unused))) {
			return errorProvider.set(ErrorProvider::ERROR_PROVIDER_TOUCH_SCREEN_CALIBRATION,E_SERIALISATION_NOT_SUPPORTED);
		}


		/*
		 * Can't deserialise
		 */

		bool PassThroughTouchScreenCalibration::deserialise(InputStream& istream __attribute__((unused))) {
			return errorProvider.set(ErrorProvider::ERROR_PROVIDER_TOUCH_SCREEN_CALIBRATION,E_SERIALISATION_NOT_SUPPORTED);
		}
	}
}
