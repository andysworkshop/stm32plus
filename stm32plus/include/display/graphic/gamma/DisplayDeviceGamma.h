/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Base class for display device gamma implementations
     *
     * A gamma configuration consists of a set of
     * 16-bit values used to configure internal registers in the controller. The number of values
     * is dependent on the controller and the values of the gammas is dependent on the panel.
     */

    class DisplayDeviceGamma {

      protected:
        Memblock<uint16_t> _gamma;
        uint16_t _lcdGammaCount;

      protected:
        DisplayDeviceGamma(int count) : _gamma(count) {
        }

      public:

        virtual ~DisplayDeviceGamma() {}

        /**
         * Get the number of gammas
         * @return The number of gammas supported by this device.
         */

        uint16_t getGammaCount() {
          return _gamma.getSize();
        }

        /**
         * [] operator. Get a modifiable gamma value.
         * @return a reference to the gamma entry
         */

        uint16_t& operator[](int pos) {
          return _gamma[pos];
        }
    };
  }
}
