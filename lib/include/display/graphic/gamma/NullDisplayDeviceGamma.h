/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * @brief gamma class that does nothing.
     *
     * Used for displays that don't support gamma correction
     */

    struct NullDisplayDeviceGamma : public DisplayDeviceGamma<NullDisplayDeviceGamma,uint8_t> {

      /**
       * Constructor
       */

      NullDisplayDeviceGamma()
        : DisplayDeviceGamma(1) {
      }


      /**
       * Get the max value per gamma
       */

      static constexpr uint8_t getMaximumValue(uint16_t /* index */) {
        return 0;
      }
    };
  }
}
