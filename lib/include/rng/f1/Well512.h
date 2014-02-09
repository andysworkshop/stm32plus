/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Implementation of the WELL512a RNG. Requires only 64 bytes of ongoing state. The default
   * constructor does a poor job of seeding the RNG.
   */

  class Well512 {

    public:
      enum {
        STATE_WORDS=16          ///< number of 32-bit words needed for state
      };

    protected:

      uint32_t state_i;
      uint32_t STATE[STATE_WORDS];
      uint32_t z0,z1,z2;

    protected:

      uint32_t MAT0POS(uint32_t v) const;
      uint32_t MAT0NEG(uint32_t t,uint32_t v) const;
      uint32_t MAT3NEG(uint32_t v) const;
      uint32_t MAT4NEG(uint32_t v) const;

    public:

      Well512();
      Well512(const uint32_t *init);

      bool nextRandom(uint32_t& nextRandom);
  };

  /**
   * Define this RNG has the default
   */

  typedef Well512 DefaultRng;


  /*
   * protected helper functions
   */

  inline uint32_t Well512::MAT0POS(uint32_t v) const {
    return (v ^ ( v >> 11));
  }

  inline uint32_t Well512::MAT0NEG(uint32_t t,uint32_t v) const {
    return (v ^ (v << t));
  }

  inline uint32_t Well512::MAT3NEG(uint32_t v) const {
    return v << 28;
  }

  inline uint32_t Well512::MAT4NEG(uint32_t v) const {
    return (v ^ ((v << 5) & 0xda442d24));
  }


  /**
   * Default constructor. Seed the RNG using the libc PRNG.
   */

  inline Well512::Well512() {

    int i;

    srand(MillisecondTimer::millis());
    for(i=0;i<STATE_WORDS;i++)
      STATE[i]=rand();

    state_i=0;
  }


  /**
   * Constructor. This allows you to set a better seed than the default.
   * @param init 16 words of random seed
   */

  inline Well512::Well512(const uint32_t *init) {
    state_i=0;
    memcpy(STATE,init,STATE_WORDS*sizeof(uint32_t));
  }


  /**
   * Produce a random number
   * @param nextRandom The random number
   * @return true always
   */

  inline bool Well512::nextRandom(uint32_t& nextRandom) {

    z0=STATE[(state_i+15) & 0xf];
    z1=MAT0NEG(16,STATE[state_i]) ^ MAT0NEG(15,STATE[(state_i+13) & 0xf]);
    z2=MAT0POS(STATE[(state_i+9) & 0xf]);

    STATE[state_i]=z1 ^ z2;
    STATE[(state_i+15) & 0xf] = MAT0NEG(2,z0) ^ MAT0NEG(18,z1) ^ MAT3NEG(z2) ^ MAT4NEG(STATE[state_i]);

    state_i=(state_i+15) & 0xf;

    nextRandom=STATE[state_i];
    return true;
  }
}
