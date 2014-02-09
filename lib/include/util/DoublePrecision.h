/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus  {

  /**
   * Carrier class for a double and it's desired output
   * precision
   */

  struct DoublePrecision {

    enum {
      MAX_DOUBLE_FRACTION_DIGITS = 5
    };

    /**
     * The encapsulated value
     */

    double Value;

    /**
     * The number of fractional digits
     */

    uint8_t Precision;


    /**
     * Constructor
     * @param d The value to encapsulate
     * @param precision The number of fractional digits
     */

    DoublePrecision(double d,uint8_t precision) {
      Value=d;
      Precision=precision;
    }
  };
}

