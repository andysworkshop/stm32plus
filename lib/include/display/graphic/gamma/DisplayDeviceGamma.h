/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Base class for display device gamma implementations
     *
     * A gamma configuration consists of a set of values (usually 16-bit but not always)
     * used to configure internal registers in the controller. The number of values
     * is dependent on the controller and the values of the gammas is dependent on the panel.
     */

    template<class TImpl,typename T>
    class DisplayDeviceGamma {

      protected:
        Memblock<T> _gamma;
        uint16_t _lcdGammaCount;

      protected:
        DisplayDeviceGamma(int count);

      public:
        uint16_t getGammaCount();
        T& operator[](int pos);
        const T& operator[](int pos) const;

        static uint16_t getMaximumValue(uint16_t index);
    };


    /**
     * Constructor
     */

    template<class TImpl,typename T>
    inline DisplayDeviceGamma<TImpl,T>::DisplayDeviceGamma(int count)
      : _gamma(count) {
    }

    /**
     * Get the number of gammas
     * @return The number of gammas supported by this device.
     */

    template<class TImpl,typename T>
    inline uint16_t DisplayDeviceGamma<TImpl,T>::getGammaCount() {
      return _gamma.getSize();
    }


    /**
     * [] operator. Get a modifiable gamma value.
     * @return a reference to the gamma entry
     */

    template<class TImpl,typename T>
    inline T& DisplayDeviceGamma<TImpl,T>::operator[](int pos) {
      return _gamma[pos];
    }


    /**
     * [] operator. Get a modifiable gamma value.
     * @return a reference to the gamma entry
     */

    template<class TImpl,typename T>
    inline const T& DisplayDeviceGamma<TImpl,T>::operator[](int pos) const {
      return _gamma[pos];
    }


    /**
     * Get the maximum value that a particular gamma value can have. Many panels
     * support only a small number of bits per value
     */

    template<class TImpl,typename T>
    inline uint16_t DisplayDeviceGamma<TImpl,T>::getMaximumValue(uint16_t index) {
      return TImpl::getMaximumValue(index);
    }
  }
}
