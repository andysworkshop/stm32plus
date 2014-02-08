/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /**
     * SSD1963 interface: 262K, 16M colours, portrait and landscape
     */

    template<class TAccessMode> using SSD1963_480x272_Portrait_262K  = GraphicsLibrary<SSD1963<PORTRAIT,COLOURS_18BIT,TAccessMode,SSD1963_480x272PanelTraits>,TAccessMode>;
    template<class TAccessMode> using SSD1963_480x272_Landscape_262K = GraphicsLibrary<SSD1963<LANDSCAPE,COLOURS_18BIT,TAccessMode,SSD1963_480x272PanelTraits>,TAccessMode>;

    template<class TAccessMode> using SSD1963_480x272_Portrait_16M  = GraphicsLibrary<SSD1963<PORTRAIT,COLOURS_24BIT,TAccessMode,SSD1963_480x272PanelTraits>,TAccessMode>;
    template<class TAccessMode> using SSD1963_480x272_Landscape_16M = GraphicsLibrary<SSD1963<LANDSCAPE,COLOURS_24BIT,TAccessMode,SSD1963_480x272PanelTraits>,TAccessMode>;

    /**
     * Graphic terminal modes for the SSD1963
     */

    template<class TDevice> using SSD1963_Terminal_Portrait = GraphicTerminal<TDevice,false>;
    template<class TDevice> using SSD1963_Terminal_Landscape = GraphicTerminal<TDevice,true>;
  }
}
