/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ili9327/commands/AllCommands.h"


namespace stm32plus {
	namespace display {


		/**
		 * Specialisation of ILI9327Orientation for the panel in LANDSCAPE mode.
		 * @tparam TAccessMode the access mode implementation, e.g. FSMC
		 */

		template<class TAccessMode,class TPanelTraits>
		class ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits> {

			private:
				TAccessMode& _accessMode;

			protected:
				ILI9327Orientation(TAccessMode& accessMode);

				constexpr uint16_t getOrientationEntryMode() const;

		  public:
				constexpr int16_t getWidth() const;
				constexpr int16_t getHeight() const;
				void moveTo(const Rectangle& rc) const;
				constexpr uint16_t getOrientationAddressMode() const;

				void setScrollPosition(int16_t scrollPosition);
		};

		/**
		 * Constructor
		 */

		template<class TAccessMode,class TPanelTraits>
		inline ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::ILI9327Orientation(TAccessMode& accessMode)
			: _accessMode(accessMode) {
		}


		/**
		 * Get the register setting for LANDSCAPE mode
		 * @return The address mode register setting for LANDSCAPE
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline uint16_t ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getOrientationAddressMode() const {
			return ili9327::SetAddressModeCmd::PAGECOL_SELECTION | ili9327::SetAddressModeCmd::HORIZONTAL_FLIP;
		}


		/**
		 * Get the width in pixels
		 * @return The width from the panel traits
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline int16_t ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getWidth() const {
		  return TPanelTraits::LONG_SIDE;
		}


		/**
		 * Get the height in pixels
		 * @return The height from the panel traits
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline int16_t ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::getHeight() const {
		  return TPanelTraits::SHORT_SIDE;
		}


		/**
		 * Move the display output rectangle
		 * @param rc The display output rectangle
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::moveTo(const Rectangle& rc) const {

			uint16_t xoffset;

			// bump past any hidden pixels

			xoffset=432-TPanelTraits::LONG_SIDE;

			this->_accessMode.writeCommand(ili9327::SetColumnAddressCmd::Opcode);
			this->_accessMode.writeData((rc.X+xoffset) >> 8);
			this->_accessMode.writeData((rc.X+xoffset) & 0xff);
			this->_accessMode.writeData((rc.X+xoffset+rc.Width-1) >>8);
			this->_accessMode.writeData((rc.X+xoffset+rc.Width-1) & 0xff);

			this->_accessMode.writeCommand(ili9327::SetPageAddressCmd::Opcode);
			this->_accessMode.writeData(rc.Y >> 8);
			this->_accessMode.writeData(rc.Y & 0xff);
			this->_accessMode.writeData((rc.Y+rc.Height-1) >>8);
			this->_accessMode.writeData((rc.Y+rc.Height-1) & 0xff);
		}


		/**
		 * Set a vertical scroll position
		 * @param scrollPosition The new scroll position
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void ILI9327Orientation<LANDSCAPE,TAccessMode,TPanelTraits>::setScrollPosition(int16_t scrollPosition) {

			uint16_t yoffset;

			if(scrollPosition<0)
        scrollPosition+=TPanelTraits::LONG_SIDE;
      else if(scrollPosition>TPanelTraits::LONG_SIDE-1)
        scrollPosition-=TPanelTraits::LONG_SIDE;

			yoffset=432-TPanelTraits::LONG_SIDE;

      // write to the register

			this->_accessMode.writeCommand(ili9327::SetScrollStartCmd::Opcode,((scrollPosition + yoffset) >> 8) & 1);
			this->_accessMode.writeData((scrollPosition + yoffset) & 0xff);
		}
	}
}

