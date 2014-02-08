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
     * ILI9327 400x240 interface: 64K and 262K colours, portrait and landscape
     */

    template<class TAccessMode> using ILI9327_400x240_Portrait_64K = GraphicsLibrary<ILI9327<PORTRAIT,COLOURS_16BIT,TAccessMode,ILI9327400x240PanelTraits>,TAccessMode>;
    template<class TAccessMode> using ILI9327_400x240_Landscape_64K = GraphicsLibrary<ILI9327<LANDSCAPE,COLOURS_16BIT,TAccessMode,ILI9327400x240PanelTraits>,TAccessMode>;

    template<class TAccessMode> using ILI9327_400x240_Portrait_262K = GraphicsLibrary<ILI9327<PORTRAIT,COLOURS_18BIT,TAccessMode,ILI9327400x240PanelTraits>,TAccessMode>;
    template<class TAccessMode> using ILI9327_400x240_Landscape_262K = GraphicsLibrary<ILI9327<LANDSCAPE,COLOURS_18BIT,TAccessMode,ILI9327400x240PanelTraits>,TAccessMode>;

    /**
     * Graphic terminal modes for the ILI9327
     */

    template<class TDevice> using ILI9327_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using ILI9327_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
