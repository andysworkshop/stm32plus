/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * ADC2 declaration
   *
   * @tparam Features A list of all the features you want to include with this ADC
   */

  template<class... Features>
  struct Adc2 : AdcPeripheral<PERIPHERAL_ADC2>,
                Features... {
    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Adc2(Adc *master=nullptr)
        : AdcPeripheral<PERIPHERAL_ADC2>(master),
          Features(static_cast<Adc&>(*this))... {

      // this is the most-derived class and construction is now complete
      // so we can initialise the peripheral

      initialisePeripheral();

      RecursiveVoidInit<Adc2,Features...>::tinit(this);

      // enable it

      enablePeripheral();
    }
  };
}
