/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {
  namespace net {


    /**
     * Random number generator for the F4 series. This uses the hardware RNG
     * to generate a sequence of 32-bit random numbers
     */

    template<class... Features>
    class Rng : public Features... {

      public:

        enum {
          E_SEED = 1,             ///< Random seed error
          E_CLOCK,                ///< Random clock error
          E_SEQUENCE_ERROR        ///< Generated number was the same as the previous one
        };

      protected:
        uint32_t _last;

      public:
        Rng();
        ~Rng();

        bool nextRandom(uint32_t& nextRandom);
    };

    /**
     * Define a feature-less Rng as the default
     */

    typedef Rng<> DefaultRng;


    /**
     * Constructor: enable the clock and start the peripheral
     */

    template<class... Features>
    inline Rng<Features...>::Rng() {

      ClockControl<PERIPHERAL_RNG>::On();
      RNG_Cmd(ENABLE);

      // FIPS-PUB 140-2: the first generated number is to be saved for comparison

      nextRandom(_last);
    }


    /**
     * Destructor
     */

    template<class... Features>
    inline Rng<Features...>::~Rng() {

      RNG_Cmd(DISABLE);
      ClockControl<PERIPHERAL_RNG>::Off();
    }


    /**
     * Generate the next random number.
     * @param nextRandom The next 32-bit random number
     * @return true if it works
     */

    template<class... Features>
    inline bool Rng<Features...>::nextRandom(uint32_t& nextRandom) {

      nextRandom=0;
      while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET) {

        if(RNG_GetFlagStatus(RNG_FLAG_CECS)==SET)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_RNG,E_CLOCK);

        if(RNG_GetFlagStatus(RNG_FLAG_SECS)==SET)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_RNG,E_SEED);
      }

      nextRandom=RNG_GetRandomNumber();
      if(nextRandom==_last)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_RNG,E_SEQUENCE_ERROR);

      _last=nextRandom;
      return true;
    }
  }
}
