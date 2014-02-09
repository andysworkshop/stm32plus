/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * DAC peripheral class
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class DacPeripheral : public Dac {

    public:

      struct Parameters {
        uint32_t dac_trigger;
        uint32_t dac_waveGeneration;
        uint32_t dac_lfsrMaskOrTriangleAmplitude;
        uint32_t dac_outputBuffer;

        /**
         * Default parameters:
         *  are no trigger (conversion triggered by loading data to the register)
         *  no wave generation
         *  bit0-11 LFSR unmask/triangle amplitude 1
         *  output buffer enabled
         */

        Parameters(uint32_t trigger=DAC_Trigger_None) {
          dac_trigger=trigger;
          dac_waveGeneration=DAC_WaveGeneration_None;
          dac_lfsrMaskOrTriangleAmplitude=DAC_LFSRUnmask_Bits11_0;
          dac_outputBuffer=DAC_OutputBuffer_Enable;
        }
      };

    protected:

      DacPeripheral(const Parameters& params);
      ~DacPeripheral();

  };


  /*
   * Constructor
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline DacPeripheral<TPinPackage,TPeripheralName>::DacPeripheral(const Parameters& params)
    : Dac(PeripheralTraits<TPeripheralName>::CHANNEL_NUMBER) {

    DAC_InitTypeDef init;

    // clocks on first before any features get initialised

    ClockControl<TPeripheralName>::On();

    // and next the GPIO pins, also before any features get initialised

    DacPinInitialiser<TPinPackage>::initialise();

    // initialise the peripheral

    DAC_StructInit(&init);

    init.DAC_Trigger=params.dac_trigger;
    init.DAC_WaveGeneration=params.dac_waveGeneration;
    init.DAC_LFSRUnmask_TriangleAmplitude=params.dac_lfsrMaskOrTriangleAmplitude;
    init.DAC_OutputBuffer=params.dac_outputBuffer;

    DAC_Init(PeripheralTraits<TPeripheralName>::CHANNEL_NUMBER,&init);

    // enable this channel

    enablePeripheral();
  }


  /*
   * Destructor
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline DacPeripheral<TPinPackage,TPeripheralName>::~DacPeripheral() {

    // disable and clocks off

    disablePeripheral();
    ClockControl<TPeripheralName>::Off();
  }
}
