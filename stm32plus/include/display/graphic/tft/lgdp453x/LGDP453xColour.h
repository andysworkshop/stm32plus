/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Empty generic class definition for LGDP453x colour-dependent operations
     * Everything is provided in the specialisations.
     */

    template<ColourDepth TColour,class TAccessMode>
    class LGDP453xColour;
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/lgdp453x/LGDP453xColour16Specialisation.h"
#include "display/graphic/tft/lgdp453x/LGDP453xColour18Specialisation.h"
