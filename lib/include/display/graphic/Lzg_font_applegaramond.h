#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_APPLE_GARAMOND_CHAR[];

  class Font_APPLE_GARAMOND_28 : public LzgFont {
    public:
      Font_APPLE_GARAMOND_28()
        : LzgFont(32,56,32,0,FDEF_APPLE_GARAMOND_CHAR) {
      }
  };
} }
