/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Empty generic class definition for LGDP453x orientation-dependent operations
     * Everything is provided in the specialisations.
     */

    template<Orientation TOrientation,class TAccessMode>
    class LGDP453xOrientation {
    };
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/lgdp453x/LGDP453xPortraitSpecialisation.h"
#include "display/graphic/tft/lgdp453x/LGDP453xLandscapeSpecialisation.h"
