#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_LINUX_LIBERTINE_CHAR[];

  class Font_LINUX_LIBERTINE_22 : public LzgFont {
    public:
      Font_LINUX_LIBERTINE_22()
        : LzgFont(32,63,25,0,FDEF_LINUX_LIBERTINE_CHAR) {
      }
  };
} }
