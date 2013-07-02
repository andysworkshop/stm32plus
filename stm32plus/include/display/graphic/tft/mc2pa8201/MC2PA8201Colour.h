/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Empty generic class definition for MC2PA8201 colour-dependent operations
     * Everything is provided in the specialisations.
     */

    template<ColourDepth TColour,class TAccessMode,class TPanelTraits>
    class MC2PA8201Colour;
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/mc2pa8201/MC2PA8201Colour16Specialisation.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201Colour18Specialisation.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201Colour24Specialisation.h"
