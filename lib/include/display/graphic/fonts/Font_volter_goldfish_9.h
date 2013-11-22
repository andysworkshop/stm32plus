#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_VOLTER__28GOLDFISH_29_CHAR[];

  class Font_VOLTER__28GOLDFISH_299 : public Font {
    public:
      Font_VOLTER__28GOLDFISH_299()
        : Font(32,95,10,1,FDEF_VOLTER__28GOLDFISH_29_CHAR) {
      }
  };
} }
