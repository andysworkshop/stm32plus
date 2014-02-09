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
     * ILI9325 interface: 64K, 262K colours, portrait and landscape
     */

    template<class TAccessMode> using ILI9325_Portrait_64K  = GraphicsLibrary<ILI9325<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using ILI9325_Landscape_64K  = GraphicsLibrary<ILI9325<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

    template<class TAccessMode> using ILI9325_Portrait_262K  = GraphicsLibrary<ILI9325<PORTRAIT,COLOURS_18BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using ILI9325_Landscape_262K  = GraphicsLibrary<ILI9325<LANDSCAPE,COLOURS_18BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the ILI9325
     */

    template<class TDevice> using ILI9325_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using ILI9325_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
