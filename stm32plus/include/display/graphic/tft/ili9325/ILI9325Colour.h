/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace display {

		/**
		 * Empty generic class definition for ILI9325 colour-dependent operations
		 * Everything is provided in the specialisations.
		 */

		template<ColourDepth TColour,class TAccessMode>
		class ILI9325Colour;
	}
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/ili9325/ILI9325Colour16Specialisation.h"
#include "display/graphic/tft/ili9325/ILI9325Colour18Specialisation.h"
