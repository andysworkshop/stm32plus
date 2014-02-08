/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Empty generic class definition for R61523 colour-dependent operations
     * Everything is provided in the specialisations.
     */

    template<ColourDepth TColour,class TAccessMode>
    class R61523Colour;
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/r61523/R61523Colour16Specialisation.h"
#include "display/graphic/tft/r61523/R61523Colour18Specialisation.h"
#include "display/graphic/tft/r61523/R61523Colour24Specialisation.h"
