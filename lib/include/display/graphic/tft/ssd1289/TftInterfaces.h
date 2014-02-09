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
     * SSD1289 interface: 64K colours, portrait and landscape
     */

    template<class TAccessMode> using SSD1289_Portrait_64K  = GraphicsLibrary<SSD1289<PORTRAIT,COLOURS_16BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using SSD1289_Landscape_64K  = GraphicsLibrary<SSD1289<LANDSCAPE,COLOURS_16BIT,TAccessMode>,TAccessMode>;

    template<class TAccessMode> using SSD1289_Portrait_262K  = GraphicsLibrary<SSD1289<PORTRAIT,COLOURS_18BIT,TAccessMode>,TAccessMode>;
    template<class TAccessMode> using SSD1289_Landscape_262K  = GraphicsLibrary<SSD1289<LANDSCAPE,COLOURS_18BIT,TAccessMode>,TAccessMode>;

    /**
     * Graphic terminal modes for the SSD1289
     */

    template<class TDevice> using SSD1289_Terminal_Portrait = GraphicTerminal<TDevice,true>;
    template<class TDevice> using SSD1289_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
