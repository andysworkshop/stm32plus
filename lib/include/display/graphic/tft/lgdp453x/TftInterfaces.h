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
     * LGDP453x interface: 64K, 262K colours, portrait and landscape
     */

    template<class TAccessMode> using LGDP453x_Portrait_64K  = GraphicsLibrary<LGDP453x<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using LGDP453x_Landscape_64K  = GraphicsLibrary<LGDP453x<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

    template<class TAccessMode> using LGDP453x_Portrait_262K  = GraphicsLibrary<LGDP453x<PORTRAIT,COLOURS_18BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using LGDP453x_Landscape_262K  = GraphicsLibrary<LGDP453x<LANDSCAPE,COLOURS_18BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the LGDP453x
     */

    template<class TDevice> using LGDP453x_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using LGDP453x_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
