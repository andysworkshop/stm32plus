/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/mc2pa8201/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Panel traits for a Type A Nokia N82 panel
     *
     * 1. Positive scroll offsets scroll downwards
     * 2. (page,column) addressing is swapped for landscape mode
     */

    class NokiaN82_TypeA {
      public:
        static int16_t normaliseScrollPosition(int16_t scrollPosition);

        template<Orientation TOrientation>
        constexpr static uint8_t getColumnAddressCommand();

        template<Orientation TOrientation>
        constexpr static uint8_t getPageAddressCommand();
    };


    /**
     * Normalise the scroll position for the setScrollPosition() call
     * @param scrollPosition the raw scroll position (0..320)
     * @return the normalised position - adjusted if necessary
     */

    inline int16_t NokiaN82_TypeA::normaliseScrollPosition(int16_t scrollPosition) {
      return scrollPosition ? 320-scrollPosition : 0;
    }


    /**
     * Get the command used to set column address in landscape mode
     * @return mc2pa8201::COLUMN_ADDRESS_SET
     */

    template<>
    constexpr inline uint8_t NokiaN82_TypeA::getColumnAddressCommand<LANDSCAPE>() {
      return mc2pa8201::COLUMN_ADDRESS_SET;
    }


    /**
     * Get the command used to set column address in landscape mode
     * @return mc2pa8201::PAGE_ADDRESS_SET
     */

    template<>
    constexpr inline uint8_t NokiaN82_TypeA::getPageAddressCommand<LANDSCAPE>() {
      return mc2pa8201::PAGE_ADDRESS_SET;
    }
  }
}
