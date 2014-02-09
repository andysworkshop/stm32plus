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
     * HX8347A interface: 64K colours, portrait and landscape
     */

    template<class TAccessMode> using HX8347A_Portrait_64K  = GraphicsLibrary<HX8347A<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using HX8347A_Landscape_64K = GraphicsLibrary<HX8347A<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the HX8347A
     */

    template<class TDevice> using HX8347A_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using HX8347A_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
