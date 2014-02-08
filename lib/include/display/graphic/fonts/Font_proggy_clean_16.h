#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_PROGGYCLEAN_CHAR[];

  class Font_PROGGYCLEAN16 : public Font {
    public:
      Font_PROGGYCLEAN16()
        : Font(32,96,14,0,FDEF_PROGGYCLEAN_CHAR) {
      }
  };
} }
