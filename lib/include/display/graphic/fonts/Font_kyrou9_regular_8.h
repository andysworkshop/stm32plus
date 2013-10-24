#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_KYROU_9_REGULAR_CHAR[];

  class Font_KYROU_9_REGULAR8 : public Font {
    public:
      Font_KYROU_9_REGULAR8()
        : Font(32,95,12,0,FDEF_KYROU_9_REGULAR_CHAR) {
      }
  };
} }
