#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_NINTENDO_DS_BIOS_CHAR[];

  class Font_NINTENDO_DS_BIOS16 : public Font {
    public:
      Font_NINTENDO_DS_BIOS16()
        : Font(32,94,11,0,FDEF_NINTENDO_DS_BIOS_CHAR) {
      }
  };
} }
