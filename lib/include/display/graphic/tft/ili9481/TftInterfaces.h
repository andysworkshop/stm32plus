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
     * ILI9481 interface: 64K colours, portrait and landscape
     */

    template<class TAccessMode> using ILI9481_Portrait_64K  = GraphicsLibrary<ILI9481<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using ILI9481_Landscape_64K  = GraphicsLibrary<ILI9481<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

    template<class TAccessMode> using ILI9481_Portrait_262K  = GraphicsLibrary<ILI9481<PORTRAIT,COLOURS_18BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using ILI9481_Landscape_262K  = GraphicsLibrary<ILI9481<LANDSCAPE,COLOURS_18BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the ILI9481
     */

    template<class TDevice> using ILI9481_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using ILI9481_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
