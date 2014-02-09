/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Forward declaration for the template specialisations. These drivers are highly optimised
     * assembly language implementations designed to extract the maximum performance from a GPIO
     * based design. Each one has been hand-tested and timed with a logic analyser to ensure it
     * meets its timing requirements.
     */

    template<class TPinPackage,ColourDepth TColourDepth,uint16_t TClockFrequency,uint16_t TLow,uint16_t THigh>
    class Gpio16BitAccessMode;
  }
}
