#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_ATARIST8X16SYSTEMFONT_CHAR[];

  class Font_ATARIST8X16SYSTEMFONT16 : public Font {
    public:
      Font_ATARIST8X16SYSTEMFONT16()
        : Font(32,96,16,0,FDEF_ATARIST8X16SYSTEMFONT_CHAR) {
      }
  };
} }
