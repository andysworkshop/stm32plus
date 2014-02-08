/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/mc2pa8201/panelTraits/Nokia6300_TypeA.h"
#include "display/graphic/tft/mc2pa8201/panelTraits/Nokia6300_TypeB.h"
#include "display/graphic/tft/mc2pa8201/panelTraits/Nokia6300_TypeC.h"


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    template<class TAccessMode> using Nokia6300_Portrait_64K_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_16BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_64K_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_16BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_262K_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_18BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_262K_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_18BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_16M_TypeA  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_24BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_16M_TypeA = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_24BIT,TAccessMode,Nokia6300_TypeA>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_64K_TypeB  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_16BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_64K_TypeB = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_16BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_262K_TypeB  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_18BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_262K_TypeB = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_18BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_16M_TypeB  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_24BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_16M_TypeB = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_24BIT,TAccessMode,Nokia6300_TypeB>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_64K_TypeC  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_16BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_64K_TypeC = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_16BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_262K_TypeC  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_18BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_262K_TypeC = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_18BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;

    template<class TAccessMode> using Nokia6300_Portrait_16M_TypeC  = GraphicsLibrary<MC2PA8201<PORTRAIT,COLOURS_24BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;
    template<class TAccessMode> using Nokia6300_Landscape_16M_TypeC = GraphicsLibrary<MC2PA8201<LANDSCAPE,COLOURS_24BIT,TAccessMode,Nokia6300_TypeC>,TAccessMode>;

    /**
     * Graphic terminal modes for the Nokia 6300
     */

    template<class TDevice> using Nokia6300_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using Nokia6300_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
