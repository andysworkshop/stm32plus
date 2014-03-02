/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


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
      AdcPeripheral(AdcOperatingMode operatingMode);
      ~AdcPeripheral();
  };


  /**
   * Constructor
   * @param params The parameters class used to initialise the peripheral
   */

  template<PeripheralName TPeripheralName>
  inline AdcPeripheral<TPeripheralName>::AdcPeripheral(AdcOperatingMode operatingMode)
    : Adc((ADC_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,operatingMode) {
  }


  /**
   * Called from the derived class constructor after the features have been constructed
   * and performed any customisation of the init and commoninit values.
   */

  template<PeripheralName TPeripheralName>
  inline void AdcPeripheral<TPeripheralName>::initialisePeripheral() {

    // clocks on first

    ClockControl<TPeripheralName>::On();

    // the features have been constructed and the common init structure customised
    // initialise it and free the memory it was using

    if(_operatingMode==AdcOperatingMode::SINGLE_ADC)
      ADC_CommonInit(_commonInit);

    delete _commonInit;

    // and now the other init call

    ADC_Init(_peripheralAddress,_init);
    delete _init;

    // if there are any injected channels then set the count

    if(_injectedChannelCount)
      ADC_InjectedSequencerLengthConfig(_peripheralAddress,_injectedChannelCount);
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
