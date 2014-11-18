/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F4_HAS_CRYPTO)
#error Incorrect MCU - this file is for the F4 devices that have hardware crypto
#endif


namespace stm32plus {


  /**
   * SHA1 implementation for the F4 processor. Basic polling operation is supported here.
   * After construction, use it like this.
   *
   * 1. Call update() zero or more times to add 32-bits of data to the hash computation.
   * 2. Call finish() for the last 32 bits or less of data that you have and receive the digest.
   * 3. To use again, call reset() and go back to step (1).
   */

  class SHA1 {

    public:
      enum {
        E_TIMED_OUT = 1,          ///< The digest operation timed out
      };

    public:
      SHA1();
      ~SHA1();

      void reset() const;
      bool hash(const void *data,uint32_t dataSize,void *hash,uint32_t timeout=0) const;
  };


  /**
   * Constructor, start the peripheral clock
   */

  inline SHA1::SHA1() {

    // clock on

    ClockControl<PERIPHERAL_HASH>::On();

    // reset and init

    reset();
  }


  /**
   * Destructor, stop the peripheral clock
   */

  inline SHA1::~SHA1() {

    // de-init and clock off

    HASH_DeInit();
    ClockControl<PERIPHERAL_HASH>::Off();
  }


  /**
   * Reset the peripheral and get it ready for a new round of hashing
   */

  inline void SHA1::reset() const {

    HASH_InitTypeDef hinit;

    // close down the peripheral

    HASH_DeInit();

    // set it up

    hinit.HASH_AlgoSelection=HASH_AlgoSelection_SHA1;
    hinit.HASH_AlgoMode=HASH_AlgoMode_HASH;
    hinit.HASH_DataType=HASH_DataType_8b;

    HASH_Init(&hinit);
  }


  /**
   * Do the hashing, get the result. After this is called you will need to call 'reset' to use
   * this class for new hash computations.
   * @param data Pointer to the data to hash
   * @param byteSize How many bytes in the 'data' pointer.
   * @param hash A pointer to at least 20 bytes of memory to receive the hash
   * @param timeout How long to wait for the computation, 0 = forever. 0 is the default.
   * @return true if it worked, false if we timed out waiting
   */

  inline bool SHA1::hash(const void *data,uint32_t dataSize,void *hash,uint32_t timeout) const {

    uint32_t start,count;
    const uint32_t *ptr;
    HASH_MsgDigest digest;

    // tell the peripheral how much is valid in the last word

    HASH_SetLastWordValidBitsNbr((dataSize % 4)*8);

    // update the digest

    ptr=reinterpret_cast<const uint32_t *>(data);
    count=dataSize/4;
    if(dataSize % 4)
      count++;

    while(count--)
      HASH_DataIn(*ptr++);

    // start the digest process

    HASH_StartDigest();

    // wait for BUSY to go low

    if(timeout)
      start=MillisecondTimer::millis();

    while(HASH_GetFlagStatus(HASH_FLAG_BUSY)!=RESET) {
      if(timeout && MillisecondTimer::hasTimedOut(start,timeout))
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_HASH,E_TIMED_OUT);
    }

    // Read the message digest

    HASH_GetDigest(&digest);
    memcpy(hash,digest.Data,20);

    return true;
  }
}
