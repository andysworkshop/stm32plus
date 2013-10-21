/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /**
     * R61523 interface: 64K colours, portrait and landscape
     */

    template<class TAccessMode> using R61523_Portrait_64K  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
//    template<class TAccessMode> using R61523_Landscape_64K  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

//    template<class TAccessMode> using R61523_Portrait_262K  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_18BIT,TAccessMode>,TAccessMode>;
//    template<class TAccessMode> using R61523_Landscape_262K  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_18BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the R61523
     */

    template<class TDevice> using R61523_Terminal_Portrait = GraphicTerminal<TDevice,false>;
//    template<class TDevice> using R61523_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
