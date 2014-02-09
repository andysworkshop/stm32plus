/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Utility class to get the prescaler values for buses
   */

  class RccBusInformation {

    public:

      /**
       * Get the value of the APB prescaler
       * @return
       */

      static uint16_t getAPBPrescaler();

      /**
       * Get the value of the APB2 prescaler
       * @return
       */

      static uint16_t getAPB2Prescaler();

      /**
       * Get the value of the APB1 prescaler
       * @return
       */

      static uint16_t getAPB1Prescaler();

      /**
       * Get the value of the AHB prescaler
       * @return
       */

      static uint16_t getAHBPrescaler();
  };
}
