/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace display {

    /**
     * @brief A Single font character definition
     *
     */

    struct FontChar {
        /// The character code
        uint8_t Code;

        /// Pixel width of this char
        uint8_t PixelWidth;

        /// Binary data packed left to right, top to bottom
        const uint8_t* Data;
    };
  }
}
