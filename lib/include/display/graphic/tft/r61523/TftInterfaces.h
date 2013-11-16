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
     * R61523 interface: 64K, 262K, 16.7M colours, portrait and landscape. Sony U5 Vivaz type A panel
     */

    template<class TAccessMode> using R61523_Portrait_64K  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_16BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_64K  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_16BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;

    template<class TAccessMode> using R61523_Portrait_262K  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_18BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_262K  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_18BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;

    template<class TAccessMode> using R61523_Portrait_16M  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_24BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_16M  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_24BIT,TAccessMode,SonyU5Vivaz_TypeA>,TAccessMode>;

    // for completeness we'll include _TypeA typedefs

    template<class TAccessMode> using R61523_Portrait_64K_TypeA=R61523_Portrait_64K<TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_64K_TypeA=R61523_Landscape_64K<TAccessMode>;
    template<class TAccessMode> using R61523_Portrait_262K_TypeA=R61523_Portrait_262K<TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_262K_TypeA=R61523_Landscape_262K<TAccessMode>;
    template<class TAccessMode> using R61523_Portrait_16M_TypeA=R61523_Portrait_16M<TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_16M_TypeA=R61523_Landscape_16M<TAccessMode>;

    /**
     * R61523 interface: 64K, 262K, 16.7M colours, portrait and landscape. Sony U5 Vivaz type B panel
     */

    template<class TAccessMode> using R61523_Portrait_64K_TypeB  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_16BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_64K_TypeB  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_16BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;

    template<class TAccessMode> using R61523_Portrait_262K_TypeB  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_18BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_262K_TypeB  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_18BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;

    template<class TAccessMode> using R61523_Portrait_16M_TypeB  = GraphicsLibrary<R61523<PORTRAIT,COLOURS_24BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;
    template<class TAccessMode> using R61523_Landscape_16M_TypeB  = GraphicsLibrary<R61523<LANDSCAPE,COLOURS_24BIT,TAccessMode,SonyU5Vivaz_TypeB>,TAccessMode>;

    /**
     * Graphic terminal modes for the R61523 (no hardware scrolling either orientation)
     */

    template<class TDevice> using R61523_Terminal_Portrait = GraphicTerminal<TDevice,false>;
    template<class TDevice> using R61523_Terminal_Landscape = GraphicTerminal<TDevice,false>;
  }
}
