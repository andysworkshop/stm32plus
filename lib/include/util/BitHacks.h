/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace bithacks {


    /**
     * Get the positional index of the first bit set in 'word'. The 32-bit multiply operation is
     * single-cycle on the m3 and m4 and possibly the m0.
     * @param word the word to scan
     * @return 0..31
     */

    inline uint8_t firstSetBit(uint32_t word) {

      static const uint8_t MultiplyDeBruijnBitPosition[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
      };

      return MultiplyDeBruijnBitPosition[((uint32_t)((word & -word) * 0x077CB531U)) >> 27];
    }


    /**
     * Reverse the ordering of bytes in a 16-bit word
     * @param data The word to swap
     * @return The word with byte ordering swapped
     */

    inline uint16_t swapBytes(uint16_t data) {

      uint16_t result;

      asm volatile( "rev16 %0, %1" : "=&r" (result) : "r" (data) );
      return result;
    }
  }
}
