#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_APPLE_CHAR[];

  class Font_APPLE8 : public Font {
    public:
      Font_APPLE8()
        : Font(32,95,8,0,FDEF_APPLE_CHAR) {
      }
  };
} }
