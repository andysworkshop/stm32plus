/*
 Copyright (c) 2011, Micael Hildenborg
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of Micael Hildenborg nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY Micael Hildenborg ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Micael Hildenborg BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 Contributors:
 Gustav
 Several members in the gamedev.se forum.
 Gregory Petrosyan
 ...and lastly Andy Brown for adapting it to the stm32plus framework for devices
 that don't have this in hardware
 */

#pragma once

#if defined(STM32PLUS_F4) && defined(STM32PLUS_F4_HARDWARE_CRYPTO)
#error Incorrect MCU - this file is for the F1 or the F4 without hardware crypto
#endif

namespace stm32plus {


  /**
   * Adaptation of the SHA1 algorithm credited in the comment above.
   */

  class SHA1 {

    protected:

      /*
       * Rotate an integer value to left.
       */

      uint32_t rol(const uint32_t value,const uint32_t steps) {
        return ((value << steps) | (value >> (32 - steps)));
      }


      /*
       * Sets the first 16 integers in the buffert to zero.
       * Used for clearing the W buffert.
       */

      void clearWBuffert(uint32_t* buffert) {
        for(int pos=16;--pos >= 0;) {
          buffert[pos]=0;
        }
      }

      void innerHash(uint32_t* result,uint32_t* w) {

        uint32_t a=result[0];
        uint32_t b=result[1];
        uint32_t c=result[2];
        uint32_t d=result[3];
        uint32_t e=result[4];

        int round=0;

  #define sha1macro(func,val) \
        { \
          const uint32_t t = rol(a, 5) + (func) + e + val + w[round]; \
          e = d; \
          d = c; \
          c = rol(b, 30); \
          b = a; \
          a = t; \
        }

        while(round < 16) {
          sha1macro((b & c) | (~b & d),0x5a827999)
          ++round;
        }
        while(round < 20) {
          w[round]=rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]),1);
          sha1macro((b & c) | (~b & d),0x5a827999)
          ++round;
        }
        while(round < 40) {
          w[round]=rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]),1);
          sha1macro(b ^ c ^ d,0x6ed9eba1)
          ++round;
        }
        while(round < 60) {
          w[round]=rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]),1);
          sha1macro((b & c) | (b & d) | (c & d),0x8f1bbcdc)
          ++round;
        }
        while(round < 80) {
          w[round]=rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]),1);
          sha1macro(b ^ c ^ d,0xca62c1d6)
          ++round;
        }

  #undef sha1macro

        result[0]+=a;
        result[1]+=b;
        result[2]+=c;
        result[3]+=d;
        result[4]+=e;
      }

    public:

      void hash(const void* src,const int bytelength,uint8_t* hash) {

        // Init the result array.
        uint32_t result[5]= { 0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0 };

        // Cast the void src pointer to be the byte array we can work with.
        const uint8_t* sarray=(const uint8_t*)src;

        // The reusable round buffer
        uint32_t w[80];

        // Loop through all complete 64byte blocks.
        const int endOfFullBlocks=bytelength - 64;
        int endCurrentBlock;
        int currentBlock=0;

        while(currentBlock <= endOfFullBlocks) {
          endCurrentBlock=currentBlock + 64;

          // Init the round buffer with the 64 byte block data.
          for(int roundPos=0;currentBlock < endCurrentBlock;currentBlock+=4) {
            // This line will swap endian on big endian and keep endian on little endian.
            w[roundPos++]=(uint32_t)sarray[currentBlock + 3] | (((uint32_t)sarray[currentBlock + 2]) << 8) | (((uint32_t)sarray[currentBlock + 1]) << 16) | (((uint32_t)sarray[currentBlock]) << 24);
          }
          innerHash(result,w);
        }

        // Handle the last and not full 64 byte block if existing.
        endCurrentBlock=bytelength - currentBlock;
        clearWBuffert(w);
        int lastBlockBytes=0;
        for(;lastBlockBytes < endCurrentBlock;++lastBlockBytes) {
          w[lastBlockBytes >> 2]|=(uint32_t)sarray[lastBlockBytes + currentBlock] << ((3 - (lastBlockBytes & 3)) << 3);
        }
        w[lastBlockBytes >> 2]|=0x80 << ((3 - (lastBlockBytes & 3)) << 3);
        if(endCurrentBlock >= 56) {
          innerHash(result,w);
          clearWBuffert(w);
        }
        w[15]=bytelength << 3;
        innerHash(result,w);

        // Store hash in result pointer, and make sure we get in in the correct order on both endian models.
        for(int hashByte=20;--hashByte >= 0;) {
          hash[hashByte]=(result[hashByte >> 2] >> (((3 - hashByte) & 0x3) << 3)) & 0xff;
        }
      }
  };
}

