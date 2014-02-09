/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/**
 * @file
 * Various string utilities. We provide integer to ASCII and optimised to-ASCII conversions for
 * double-precision and uint32_t values.
 */

namespace stm32plus {
  namespace StringUtil {

    char* itoa(int value,char* result,int base);
    uint32_t modp_uitoa10(uint32_t value,char *str);
    uint16_t modp_dtoa(double value,int8_t prec,char *str);
    void toHex(const void *buffer,uint32_t bufferSize,char *output);
  }
}
