#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_PERFECT_DOS_VGA_437_WIN_CHAR[];

  class Font_PERFECT_DOS_VGA_437_WIN16 : public Font {
    public:
      Font_PERFECT_DOS_VGA_437_WIN16()
        : Font(32,143,15,0,FDEF_PERFECT_DOS_VGA_437_WIN_CHAR) {
      }
  };
} }
