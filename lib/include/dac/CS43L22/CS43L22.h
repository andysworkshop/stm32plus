/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Cirrus Logic CS43L22 DAC implementation. The control peripheral is usually an I2C
   * class such as CS43L22ControlI2C. The data peripheral is usually an I2S class, but
   * there is a dummy default for the times when you just want the Analog pass
   * through's and will not be supplying digital data.
   */

  template<class TControlPeripheral,class TDataPeripheral=NullCS43L22DataPeripheral>
  class CS43L22 : public TControlPeripheral,public TDataPeripheral {

    public:

      /**
       * CS43L22 parameters class. This inherits from the parameters classes defined by
       * whatever the base classes require.
       */

      struct Parameters : TControlPeripheral::Parameters, TDataPeripheral::Parameters {

        /**
         * Reference to the reset pin (digital output)
         */

        GpioPinRef resetPin;
      };

    protected:
      GpioPinRef _resetPin;

    public:
      CS43L22(const Parameters& params);

      void reset() const;
  };


  /**
   * Constructor
   * @param params The parameters class containing the reset pin
   */

  template<class TControlPeripheral,class TDataPeripheral>
  inline CS43L22<TControlPeripheral,TDataPeripheral>::CS43L22(const Parameters& params)
    : TControlPeripheral(params),
      TDataPeripheral(params),
      _resetPin(params.resetPin) {
  }


  /**
   * Reset the device
   */

  template<class TControlPeripheral,class TDataPeripheral>
  inline void CS43L22<TControlPeripheral,TDataPeripheral>::reset() const {

    // minimum reset hold time = 1ms

    _resetPin.reset();
    MillisecondTimer::delay(2);
    _resetPin.set();
  }
}
