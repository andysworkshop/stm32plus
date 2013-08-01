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
		 * HX8352A interface: 64K colours, portrait and landscape
		 */

		template<class TAccessMode,class TPanelTraits> using HX8352A_Portrait_64K  = GraphicsLibrary<HX8352A<PORTRAIT,COLOURS_16BIT,TAccessMode,TPanelTraits>,TAccessMode>;
		template<class TAccessMode,class TPanelTraits> using HX8352A_Landscape_64K = GraphicsLibrary<HX8352A<LANDSCAPE,COLOURS_16BIT,TAccessMode,TPanelTraits>,TAccessMode>;

		/**
		 * HX8352A interface: 262K colours, portrait and landscape
		 */

		template<class TAccessMode,class TPanelTraits> using HX8352A_Portrait_262K  = GraphicsLibrary<HX8352A<PORTRAIT,COLOURS_18BIT,TAccessMode,TPanelTraits>,TAccessMode>;
		template<class TAccessMode,class TPanelTraits> using HX8352A_Landscape_262K = GraphicsLibrary<HX8352A<LANDSCAPE,COLOURS_18BIT,TAccessMode,TPanelTraits>,TAccessMode>;

		/**
		 * Graphic terminal modes for the HX8352A
		 */

		template<class TDevice> using HX8352A_Terminal_Portrait = GraphicTerminal<TDevice,true>;
		template<class TDevice> using HX8352A_Terminal_Landscape = GraphicTerminal<TDevice,false>;


		/**
		 * Specialisations for the LG KF700 480x240 panel
		 */

		template<class TAccessMode> using LG_KF700_Portrait_64K =  HX8352A_Portrait_64K<TAccessMode,LG_KF700>;
		template<class TAccessMode> using LG_KF700_Landscape_64K =  HX8352A_Landscape_64K<TAccessMode,LG_KF700>;

		template<class TAccessMode> using LG_KF700_Portrait_262K =  HX8352A_Portrait_262K<TAccessMode,LG_KF700>;
		template<class TAccessMode> using LG_KF700_Landscape_262K =  HX8352A_Landscape_262K<TAccessMode,LG_KF700>;

		template<class TDevice> using LG_KF700_Terminal_Portrait = GraphicTerminal<TDevice,true>;
		template<class TDevice> using LG_KF700_Terminal_Landscape = GraphicTerminal<TDevice,false>;
	}
}
