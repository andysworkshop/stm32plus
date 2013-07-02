/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @namespace display 'display' is a sub-namespace of stm32plus. Everything to do with graphics libraries
   * and drivers is in this namespace
   */

  namespace display {

    /**
     * @brief A pixel size structure
     */

    struct Size {

        /// The width
        int16_t Width;

        /// The height
        int16_t Height;

        Size() {}

        /**
         * Constructor
         * @param[in] width The width parameter.
         * @param[in] height The height parameter.
         */

        Size(int16_t width,int16_t height) :
          Width(width), Height(height) {
        }
    };
  }
}
