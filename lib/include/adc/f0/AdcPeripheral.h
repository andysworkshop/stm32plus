/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Template base class for ADC peripherals.
   *
   * @tparam TPeripheralName The peripheral name (PERIPHERAL_ADC1, PERIPHERAL_ADC2, PERIPHERAL_ADC3)
   */

  template<PeripheralName TPeripheralName>
  class AdcPeripheral : public Adc {

    protected:
      void initialisePeripheral();

    public:
      AdcPeripheral(Adc *master);
      ~AdcPeripheral();
  };


  /**
   * Constructor
   * @param params The parameters class used to initialise the peripheral
   */

  template<PeripheralName TPeripheralName>
  inline AdcPeripheral<TPeripheralName>::AdcPeripheral(Adc * /* master */) {
  }


  /**
   * Called from the derived class constructor after the features have been constructed
   * and performed any customisation of the init and commoninit values.
   */

  template<PeripheralName TPeripheralName>
  inline void AdcPeripheral<TPeripheralName>::initialisePeripheral() {

    // clocks on first

    ClockControl<TPeripheralName>::On();

    // the peripheral must be disabled for auto-calibration to work

    ADC_Cmd(ADC1,DISABLE);
    ADC_DeInit(ADC1);

    // start the calibration

    ADC_GetCalibrationFactor(ADC1);

    // the features have been constructed, initialise it and
    // free the memory it was using

    ADC_Init(ADC1,_init);
    delete _init;
  }


  /**
   * Destructor, disable peripheral and disable clocks
   */

  template<PeripheralName TPeripheralName>
  inline AdcPeripheral<TPeripheralName>::~AdcPeripheral() {

    // disable the peripheral

    disablePeripheral();

    // clocks off

    ClockControl<TPeripheralName>::Off();
  }
}
