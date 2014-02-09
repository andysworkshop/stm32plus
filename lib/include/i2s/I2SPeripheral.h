/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * I2S peripheral class
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class I2SPeripheral : public I2S {

    public:

      /**
       * I2S parameters class
       */

      struct Parameters {
        uint32_t i2s_audioFrequency;
        uint16_t i2s_mode;
        uint16_t i2s_standard;
        uint16_t i2s_dataFormat;
        uint16_t i2s_mclkOutput;
        uint16_t i2s_cpol;

        Parameters(uint32_t audioFrequency=I2S_AudioFreq_44k) {
          i2s_audioFrequency=audioFrequency;
          i2s_mode=I2S_Mode_MasterTx;
          i2s_standard=I2S_Standard_Phillips;
          i2s_dataFormat=I2S_DataFormat_16b;
          i2s_mclkOutput=I2S_MCLKOutput_Enable;
          i2s_cpol=I2S_CPOL_Low;
        }
      };

    protected:

      I2SPeripheral(const struct Parameters& params);
      ~I2SPeripheral();
  };


  /**
   * Constructor
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline I2SPeripheral<TPinPackage,TPeripheralName>::I2SPeripheral(const struct Parameters& params)
    : I2S((SPI_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE) {

    I2S_InitTypeDef init;

    // clocks on first before any features get initialised

    ClockControl<TPeripheralName>::On();

    // and next the GPIO pins, also before any features get initialised

    I2SPinInitialiser<TPinPackage,TPeripheralName>::initialise(params.i2s_mode,params.i2s_mclkOutput);

    // initialise the peripheral

    I2S_StructInit(&init);

    init.I2S_Mode=params.i2s_mode;
    init.I2S_Standard=params.i2s_standard;
    init.I2S_DataFormat=params.i2s_dataFormat;
    init.I2S_MCLKOutput=params.i2s_mclkOutput;
    init.I2S_AudioFreq=params.i2s_audioFrequency;
    init.I2S_CPOL=params.i2s_cpol;

    // initialise the peripheral

    I2S_Init(_peripheralAddress,&init);
    enablePeripheral();
  }


  /**
   * Destructor
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline I2SPeripheral<TPinPackage,TPeripheralName>::~I2SPeripheral() {

    // disable and clocks off

    disablePeripheral();
    ClockControl<TPeripheralName>::Off();
  }
}
