/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8352a/commands/AllCommands.h"


namespace stm32plus {
	namespace display {


		/**
		 * Specialisation of HX8352AOrientation for the panel in PORTRAIT mode.
		 * @tparam TAccessMode the access mode implementation, e.g. FSMC
		 */

		template<class TAccessMode,class TPanelTraits>
		class HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits> {

			private:
				TAccessMode& _accessMode;

			protected:
				HX8352AOrientation(TAccessMode& accessMode);

				constexpr uint16_t getMemoryAccessControl() const;

		  public:
				constexpr int16_t getWidth() const;
				constexpr int16_t getHeight() const;
				void moveTo(const Rectangle& rc) const;

				void setScrollPosition(int16_t scrollPosition);
		};


		/**
		 * Constructor
		 */

		template<class TAccessMode,class TPanelTraits>
		inline HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::HX8352AOrientation(TAccessMode& accessMode)
			: _accessMode(accessMode) {
		}


		/**
		 * Get the register setting for memory access control
		 * @return The entry mode register setting for portrait
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline uint16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getMemoryAccessControl() const {
			return 0x8;		// BGR
		}


		/**
		 * Get the width in pixels
		 * @return the panel's width
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline int16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getWidth() const {
		  return TPanelTraits::SHORT_SIDE;
		}


		/**
		 * Get the height in pixels
		 * @return the panel's height
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline int16_t HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::getHeight() const {
		  return TPanelTraits::LONG_SIDE;
		}


		/**
		 * Move the display output rectangle
		 * @param rc The display output rectangle
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {

			this->_accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_H,rc.X >> 8);
			this->_accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_L,rc.X & 0xff);
			this->_accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_H,(rc.X+rc.Width-1) >> 8);
			this->_accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_L,(rc.X+rc.Width-1) & 0xff);

			this->_accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_H,rc.Y >> 8);
			this->_accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_L,rc.Y & 0xff);
			this->_accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_H,(rc.Y+rc.Height-1) >> 8);
			this->_accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_L,(rc.Y+rc.Height-1) & 0xff);
		}


		/**
		 * Set a vertical scroll position
		 * @param scrollPosition The new scroll position
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void HX8352AOrientation<PORTRAIT,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) {

			if(scrollPosition<0)
        scrollPosition+=TPanelTraits::LONG_SIDE;
      else if(scrollPosition>TPanelTraits::LONG_SIDE-1)
        scrollPosition-=TPanelTraits::LONG_SIDE;

      // write to the register

			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_START_ADDRESS_H,(scrollPosition >> 8));
			this->_accessMode.writeCommand(hx8352a::VERTICAL_SCROLL_START_ADDRESS_L,scrollPosition & 0xff);
		}
	}
}

