/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {
    namespace NetUtil {


      /**
       * Reverse the bytes in the parameter. We can do this in a single instruction
       * @param data
       * @return The parameter, with bits reversed
       */

      __attribute__((always_inline)) inline static uint16_t ntohs(uint16_t data) {

        uint16_t result;

        asm volatile( "rev16 %0, %1" : "=&r" (result) : "r" (data) );
        return result;
      }


      /**
       * Reverse the bytes in the parameter. We can do this in a single instruction
       * @param data
       * @return The parameter, with bits reversed
       */

      __attribute__((always_inline)) inline static uint32_t ntohl(uint32_t data) {

        uint32_t result;

        asm volatile( "rev %0, %1" : "=&r" (result) : "r" (data) );
        return result;
      }


      /**
       * Reverse the bytes in the parameter. We can do this in a single instruction
       * @param data
       * @return The parameter, with bits reversed
       */

      __attribute__((always_inline)) inline static uint16_t htons(uint16_t data) {

        uint16_t result;

        asm volatile( "rev16 %0, %1" : "=&r" (result) : "r" (data) );
        return result;
      }


      /**
       * Reverse the bytes in the parameter. We can do this in a single instruction
       * @param data
       * @return The parameter, with bits reversed
       */

      __attribute__((always_inline)) inline static uint32_t htonl(uint32_t data) {

        uint32_t result;

        asm volatile( "rev %0, %1" : "=&r" (result) : "r" (data) );
        return result;
      }
    }
  }
}
