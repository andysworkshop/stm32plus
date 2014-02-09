/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/mc2pa8201/panelTraits/NokiaE73_TypeA.h"


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /**
     * Nokia E73 interface: 64K, 262K, 16M colours, portrait and landscape
     */

    template<class TAccessMode> using NokiaE73_Portrait_64K_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_16BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaE73_Landscape_64K_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_16BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;

    template<class TAccessMode> using NokiaE73_Portrait_262K_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_18BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaE73_Landscape_262K_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_18BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;

    template<class TAccessMode> using NokiaE73_Portrait_16M_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_24BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaE73_Landscape_16M_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_24BIT,TAccessMode,NokiaE73_TypeA>,TAccessMode>;

    /**
     * Graphic terminal modes for the Nokia E73
     */

    template<class TDevice> using NokiaE73_Terminal_Portrait = GraphicTerminal<TDevice,false>;
    template<class TDevice> using NokiaE73_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
