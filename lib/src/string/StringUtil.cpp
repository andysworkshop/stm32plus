/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/string.h"


namespace stm32plus {

  namespace StringUtil {

    /**
     * Convert input values to lower case hex (like bin2hex).
     * @param buffer The input values
     * @param bufferSize How many bytes to convert
     * @param output Where to store output. Requires 2x bufferSize bytes available.
     */

    void toHex(const void *buffer,uint32_t bufferSize,char *output) {

      const char *ptr;
      static const char *hexSet="0123456789abcdef";

      ptr=reinterpret_cast<const char *>(buffer);
      while(bufferSize--) {

        *output++=hexSet[(*ptr & 0xf0) >> 4];
        *output++=hexSet[*ptr & 0xf];
        ptr++;
      }
    }


    /*
     * itoa - not present in standard libraries
     */

    char* itoa(int value,char* result,int base) {

      char* ptr=result,*ptr1=result,tmp_char;
      int tmp_value;

      do {
        tmp_value=value;
        value/=base;
        *ptr++="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35+(tmp_value-value*base)];
      } while(value);

      // Apply negative sign

      if(tmp_value<0)
        *ptr++='-';

      *ptr--='\0';

      while(ptr1<ptr) {
        tmp_char=*ptr;
        *ptr--=*ptr1;
        *ptr1++=tmp_char;
      }
      return result;
    }


    /*
     * modp_uitoa10 (from stringencoders library)
     */

    uint32_t modp_uitoa10(uint32_t value,char *str) {

      char *wstr=str;
      uint32_t size;

      // Conversion. Number is reversed.

      do *wstr++ = (char)(48 + (value % 10)); while (value /= 10);
      *wstr='\0';

      // andy's mod: reverse the string in place. this is probably optimal.

      size=wstr-str;
      wstr--;

      while(str<wstr) {

        *str^=*wstr;
        *wstr^=*str;
        *str^=*wstr;

        wstr--;
        str++;
      }

      return size;
    }


    /*
     * Derived from http://code.google.com/p/stringencoders/source/browse/trunk/src/modp_numtoa.c
     * "string encoders: A collection of high performance c-string transformations".
     *
     * The following license applies:
     *
     * MODP_B64 - High performance base64 encoder/decoder
     * http://code.google.com/p/stringencoders/
     *
     * Copyright (c) 2005, 2006, 2007  Nick Galbreath -- nickg [at] modp [dot] com
     * All rights reserved.
     *
     * Redistribution and use in source and binary forms, with or without
     * modification, are permitted provided that the following conditions are
     * met:
     *
     *   Redistributions of source code must retain the above copyright
     *   notice, this list of conditions and the following disclaimer.
     *
     *   Redistributions in binary form must reproduce the above copyright
     *   notice, this list of conditions and the following disclaimer in the
     *   documentation and/or other materials provided with the distribution.
     *
     *   Neither the name of the modp.com nor the names of its
     *   contributors may be used to endorse or promote products derived from
     *   this software without specific prior written permission.
     *
     * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     *
     * This is the standard "new" BSD license:
     * http://www.opensource.org/licenses/bsd-license.php
     */

    uint16_t modp_dtoa(double value,int8_t prec,char *str) {

      static const double pow10[] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
      uint16_t length;

      if(!(value==value)) {

        str[0]='n';
        str[1]='a';
        str[2]='n';
        str[3]='\0';
        return 3;
      }

      int8_t count;
      double diff=0.0;
      char *wstr=str;

    // we'll work in positive values and deal with the negative sign issue later

      bool neg=false;

      if(value<0) {
        neg=true;
        value=-value;
      }

      uint32_t whole=(uint32_t)value;
      double tmp=(value-whole)*pow10[prec];
      uint32_t frac=(uint32_t)(tmp);

      diff=tmp-frac;

      if(diff>0.5) {

        ++frac;

        // handle rollover, e.g.  case 0.99 with prec 1 is 1.0

        if(frac>=pow10[prec]) {
          frac=0;
          ++whole;
        }

      } else if(diff==0.5 && ((frac==0) || (frac & 1))) {

        // if halfway, round up if odd, OR if last digit is 0.  That last part is strange

        ++frac;
      }

      if(prec==0) {

        diff=value-whole;

        if(diff>0.5) {
        // greater than 0.5, round up, e.g. 1.6 -> 2

          ++whole;

        } else if(diff==0.5 && (whole & 1)) {

          // exactly 0.5 and ODD, then round up
          // 1.5 -> 2, but 2.5 -> 2 */

          ++whole;
        }

      } else if(frac) {

        count=prec;

        // now do fractional part, as an unsigned number
        // we know it is not 0 but we can have leading zeros, these should be removed

        while(!(frac % 10)) {
          --count;
          frac/=10;
        }

      // now do fractional part, as an unsigned number

        do {
          --count;
          *wstr++=(char)(48 + (frac % 10));
        } while(frac/=10);

        // add extra 0s

        while(count-- > 0)
          *wstr++='0';

        // add decimal

        *wstr++='.';
      }

      // do whole part. Take care of sign conversion. Number is reversed.

      do {
        *wstr++=(char)(48 + (whole % 10));
      } while(whole/=10);

      if(neg)
        *wstr++='-';

      *wstr='\0';
      length=wstr-str;

      // andy's mod: reverse the string in place. this is probably optimal.

      wstr--;

      while(str<wstr) {

        *str^=*wstr;
        *wstr^=*str;
        *str^=*wstr;

        wstr--;
        str++;
      }

      return length;
    }
  }
}
