#pragma once

#include "Font.h"

namespace stm32plus { namespace display {

  // byte definitions for FDEF_APPLE

  const uint8_t FDEF_APPLE48_BYTES[]={ 28,34,50,42,38,34,28,0,};
  const uint8_t FDEF_APPLE49_BYTES[]={ 8,12,8,8,8,8,28,0,};
  const uint8_t FDEF_APPLE50_BYTES[]={ 28,34,32,24,4,2,62,0,};
  const uint8_t FDEF_APPLE51_BYTES[]={ 62,32,16,24,32,34,28,0,};
  const uint8_t FDEF_APPLE52_BYTES[]={ 16,24,20,18,62,16,16,0,};
  const uint8_t FDEF_APPLE53_BYTES[]={ 62,2,30,32,32,34,28,0,};
  const uint8_t FDEF_APPLE54_BYTES[]={ 56,4,2,30,34,34,28,0,};
  const uint8_t FDEF_APPLE55_BYTES[]={ 62,32,16,8,4,4,4,0,};
  const uint8_t FDEF_APPLE56_BYTES[]={ 28,34,34,28,34,34,28,0,};
  const uint8_t FDEF_APPLE57_BYTES[]={ 28,34,34,60,32,16,14,0,};
  const uint8_t FDEF_APPLE65_BYTES[]={ 8,20,34,34,62,34,34,0,};
  const uint8_t FDEF_APPLE66_BYTES[]={ 30,34,34,30,34,34,30,0,};
  const uint8_t FDEF_APPLE67_BYTES[]={ 28,34,2,2,2,34,28,0,};
  const uint8_t FDEF_APPLE68_BYTES[]={ 30,34,34,34,34,34,30,0,};
  const uint8_t FDEF_APPLE69_BYTES[]={ 62,2,2,30,2,2,62,0,};
  const uint8_t FDEF_APPLE70_BYTES[]={ 62,2,2,30,2,2,2,0,};
  const uint8_t FDEF_APPLE71_BYTES[]={ 60,2,2,2,50,34,60,0,};
  const uint8_t FDEF_APPLE72_BYTES[]={ 34,34,34,62,34,34,34,0,};
  const uint8_t FDEF_APPLE73_BYTES[]={ 28,8,8,8,8,8,28,0,};
  const uint8_t FDEF_APPLE74_BYTES[]={ 32,32,32,32,32,34,28,0,};
  const uint8_t FDEF_APPLE75_BYTES[]={ 34,18,10,6,10,18,34,0,};
  const uint8_t FDEF_APPLE76_BYTES[]={ 2,2,2,2,2,2,62,0,};
  const uint8_t FDEF_APPLE77_BYTES[]={ 34,54,42,42,34,34,34,0,};
  const uint8_t FDEF_APPLE78_BYTES[]={ 34,34,38,42,50,34,34,0,};
  const uint8_t FDEF_APPLE79_BYTES[]={ 28,34,34,34,34,34,28,0,};
  const uint8_t FDEF_APPLE80_BYTES[]={ 30,34,34,30,2,2,2,0,};
  const uint8_t FDEF_APPLE81_BYTES[]={ 28,34,34,34,42,18,44,0,};
  const uint8_t FDEF_APPLE82_BYTES[]={ 30,34,34,30,10,18,34,0,};
  const uint8_t FDEF_APPLE83_BYTES[]={ 28,34,2,28,32,34,28,0,};
  const uint8_t FDEF_APPLE84_BYTES[]={ 62,8,8,8,8,8,8,0,};
  const uint8_t FDEF_APPLE85_BYTES[]={ 34,34,34,34,34,34,28,0,};
  const uint8_t FDEF_APPLE86_BYTES[]={ 34,34,34,34,34,20,8,0,};
  const uint8_t FDEF_APPLE87_BYTES[]={ 34,34,34,42,42,54,34,0,};
  const uint8_t FDEF_APPLE88_BYTES[]={ 34,34,20,8,20,34,34,0,};
  const uint8_t FDEF_APPLE89_BYTES[]={ 34,34,20,8,8,8,8,0,};
  const uint8_t FDEF_APPLE90_BYTES[]={ 62,32,16,8,4,2,62,0,};
  const uint8_t FDEF_APPLE97_BYTES[]={ 0,0,28,32,60,34,60,0,};
  const uint8_t FDEF_APPLE98_BYTES[]={ 2,2,30,34,34,34,30,0,};
  const uint8_t FDEF_APPLE99_BYTES[]={ 0,0,60,2,2,2,60,0,};
  const uint8_t FDEF_APPLE100_BYTES[]={ 32,32,60,34,34,34,60,0,};
  const uint8_t FDEF_APPLE101_BYTES[]={ 0,0,28,34,62,2,60,0,};
  const uint8_t FDEF_APPLE102_BYTES[]={ 24,36,4,30,4,4,4,0,};
  const uint8_t FDEF_APPLE103_BYTES[]={ 0,0,28,34,34,60,32,28,};
  const uint8_t FDEF_APPLE104_BYTES[]={ 2,2,30,34,34,34,34,0,};
  const uint8_t FDEF_APPLE105_BYTES[]={ 8,0,12,8,8,8,28,0,};
  const uint8_t FDEF_APPLE106_BYTES[]={ 16,0,24,16,16,16,18,12,};
  const uint8_t FDEF_APPLE107_BYTES[]={ 2,2,34,18,14,18,34,0,};
  const uint8_t FDEF_APPLE108_BYTES[]={ 12,8,8,8,8,8,28,0,};
  const uint8_t FDEF_APPLE109_BYTES[]={ 0,0,54,42,42,42,34,0,};
  const uint8_t FDEF_APPLE110_BYTES[]={ 0,0,30,34,34,34,34,0,};
  const uint8_t FDEF_APPLE111_BYTES[]={ 0,0,28,34,34,34,28,0,};
  const uint8_t FDEF_APPLE112_BYTES[]={ 0,0,30,34,34,34,30,2,};
  const uint8_t FDEF_APPLE113_BYTES[]={ 0,0,60,34,34,34,60,32,};
  const uint8_t FDEF_APPLE114_BYTES[]={ 0,0,58,6,2,2,2,0,};
  const uint8_t FDEF_APPLE115_BYTES[]={ 0,0,60,2,28,32,30,0,};
  const uint8_t FDEF_APPLE116_BYTES[]={ 8,8,60,8,8,72,48,0,};
  const uint8_t FDEF_APPLE117_BYTES[]={ 0,0,34,34,34,50,44,0,};
  const uint8_t FDEF_APPLE118_BYTES[]={ 0,0,34,34,34,20,8,0,};
  const uint8_t FDEF_APPLE119_BYTES[]={ 0,0,34,34,42,42,54,0,};
  const uint8_t FDEF_APPLE120_BYTES[]={ 0,0,34,20,8,20,34,0,};
  const uint8_t FDEF_APPLE121_BYTES[]={ 0,0,34,34,34,60,32,28,};
  const uint8_t FDEF_APPLE122_BYTES[]={ 0,0,62,16,8,4,62,0,};

  // character definitions for FDEF_APPLE

  const struct FontChar FDEF_APPLE_CHAR[]={
    { 48,8,FDEF_APPLE48_BYTES },
    { 49,8,FDEF_APPLE49_BYTES },
    { 50,8,FDEF_APPLE50_BYTES },
    { 51,8,FDEF_APPLE51_BYTES },
    { 52,8,FDEF_APPLE52_BYTES },
    { 53,8,FDEF_APPLE53_BYTES },
    { 54,8,FDEF_APPLE54_BYTES },
    { 55,8,FDEF_APPLE55_BYTES },
    { 56,8,FDEF_APPLE56_BYTES },
    { 57,8,FDEF_APPLE57_BYTES },
    { 65,8,FDEF_APPLE65_BYTES },
    { 66,8,FDEF_APPLE66_BYTES },
    { 67,8,FDEF_APPLE67_BYTES },
    { 68,8,FDEF_APPLE68_BYTES },
    { 69,8,FDEF_APPLE69_BYTES },
    { 70,8,FDEF_APPLE70_BYTES },
    { 71,8,FDEF_APPLE71_BYTES },
    { 72,8,FDEF_APPLE72_BYTES },
    { 73,8,FDEF_APPLE73_BYTES },
    { 74,8,FDEF_APPLE74_BYTES },
    { 75,8,FDEF_APPLE75_BYTES },
    { 76,8,FDEF_APPLE76_BYTES },
    { 77,8,FDEF_APPLE77_BYTES },
    { 78,8,FDEF_APPLE78_BYTES },
    { 79,8,FDEF_APPLE79_BYTES },
    { 80,8,FDEF_APPLE80_BYTES },
    { 81,8,FDEF_APPLE81_BYTES },
    { 82,8,FDEF_APPLE82_BYTES },
    { 83,8,FDEF_APPLE83_BYTES },
    { 84,8,FDEF_APPLE84_BYTES },
    { 85,8,FDEF_APPLE85_BYTES },
    { 86,8,FDEF_APPLE86_BYTES },
    { 87,8,FDEF_APPLE87_BYTES },
    { 88,8,FDEF_APPLE88_BYTES },
    { 89,8,FDEF_APPLE89_BYTES },
    { 90,8,FDEF_APPLE90_BYTES },
    { 97,8,FDEF_APPLE97_BYTES },
    { 98,8,FDEF_APPLE98_BYTES },
    { 99,8,FDEF_APPLE99_BYTES },
    { 100,8,FDEF_APPLE100_BYTES },
    { 101,8,FDEF_APPLE101_BYTES },
    { 102,8,FDEF_APPLE102_BYTES },
    { 103,8,FDEF_APPLE103_BYTES },
    { 104,8,FDEF_APPLE104_BYTES },
    { 105,8,FDEF_APPLE105_BYTES },
    { 106,8,FDEF_APPLE106_BYTES },
    { 107,8,FDEF_APPLE107_BYTES },
    { 108,8,FDEF_APPLE108_BYTES },
    { 109,8,FDEF_APPLE109_BYTES },
    { 110,8,FDEF_APPLE110_BYTES },
    { 111,8,FDEF_APPLE111_BYTES },
    { 112,8,FDEF_APPLE112_BYTES },
    { 113,8,FDEF_APPLE113_BYTES },
    { 114,8,FDEF_APPLE114_BYTES },
    { 115,8,FDEF_APPLE115_BYTES },
    { 116,8,FDEF_APPLE116_BYTES },
    { 117,8,FDEF_APPLE117_BYTES },
    { 118,8,FDEF_APPLE118_BYTES },
    { 119,8,FDEF_APPLE119_BYTES },
    { 120,8,FDEF_APPLE120_BYTES },
    { 121,8,FDEF_APPLE121_BYTES },
    { 122,8,FDEF_APPLE122_BYTES },
  };

  // helper so the user can just do 'new fontname' without having to know the parameters

  class Font_APPLE8 : public Font {
    public:
      Font_APPLE8()
        : Font(48,62,8,0,FDEF_APPLE_CHAR) {
      }
  };
} }
