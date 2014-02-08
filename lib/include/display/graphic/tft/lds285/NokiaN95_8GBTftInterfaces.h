/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/lds285/panelTraits/NokiaN95_8GB_TypeA.h"
#include "display/graphic/tft/lds285/panelTraits/NokiaN95_8GB_TypeB.h"


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /**
     * Nokia N95 8GB Type A interface: 64K, 262K, 16M colours, portrait and landscape
     */

    template<class TAccessMode> using NokiaN95_8GB_Portrait_64K_TypeA  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_16BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_64K_TypeA  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_16BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;

    template<class TAccessMode> using NokiaN95_8GB_Portrait_262K_TypeA  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_18BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_262K_TypeA  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_18BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;

    template<class TAccessMode> using NokiaN95_8GB_Portrait_16M_TypeA  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_24BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_16M_TypeA  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_24BIT,TAccessMode,NokiaN95_8GB_TypeA>,TAccessMode>;

    /**
     * Nokia N95 8GB Type B interface: 64K, 262K, 16M colours, portrait and landscape
     */

    template<class TAccessMode> using NokiaN95_8GB_Portrait_64K_TypeB  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_16BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_64K_TypeB  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_16BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;

    template<class TAccessMode> using NokiaN95_8GB_Portrait_262K_TypeB  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_18BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_262K_TypeB  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_18BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;

    template<class TAccessMode> using NokiaN95_8GB_Portrait_16M_TypeB  = GraphicsLibrary<LDS285<PORTRAIT,COLOURS_24BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;
    template<class TAccessMode> using NokiaN95_8GB_Landscape_16M_TypeB  = GraphicsLibrary<LDS285<LANDSCAPE,COLOURS_24BIT,TAccessMode,NokiaN95_8GB_TypeB>,TAccessMode>;

    /**
     * Graphic terminal modes for the NokiaN95_8GB
     */

    template<class TDevice> using NokiaN95_8GB_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using NokiaN95_8GB_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
