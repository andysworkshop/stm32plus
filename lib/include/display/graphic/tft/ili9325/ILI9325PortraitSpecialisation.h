/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ili9325/commands/AllCommands.h"


namespace stm32plus {
	namespace display {


		/**
		 * Specialisation of ILI9325Orientation for the panel in PORTRAIT mode.
		 * @tparam TAccessMode the access mode implementation, e.g. FSMC
		 */

		template<class TAccessMode>
		class ILI9325Orientation<PORTRAIT,TAccessMode> {

			private:
				TAccessMode& _accessMode;

			protected:
				ILI9325Orientation(TAccessMode& accessMode);

				constexpr uint16_t getOrientationEntryMode() const;

		  public:
				constexpr int16_t getWidth() const;
				constexpr int16_t getHeight() const;
				void moveTo(const Rectangle& rc) const;

				void setScrollPosition(int16_t scrollPosition);
		};


		/**
		 * Constructor
		 */

		template<class TAccessMode>
		inline ILI9325Orientation<PORTRAIT,TAccessMode>::ILI9325Orientation(TAccessMode& accessMode)
			: _accessMode(accessMode) {
		}


		/**
		 * Get the register setting for portrait mode
		 * @return The entry mode register setting for portrait
		 */

		template<class TAccessMode>
		constexpr inline uint16_t ILI9325Orientation<PORTRAIT,TAccessMode>::getOrientationEntryMode() const {
			return ili9325::EntryModeCmd::AM_HORIZONTAL | ili9325::EntryModeCmd::ID_HINC_VINC;
		}


		/**
		 * Get the width in pixels
		 * @return 240px
		 */

		template<class TAccessMode>
		constexpr inline int16_t ILI9325Orientation<PORTRAIT,TAccessMode>::getWidth() const {
		  return 240;
		}


		/**
		 * Get the height in pixels
		 * @return 320px
		 */

		template<class TAccessMode>
		constexpr inline int16_t ILI9325Orientation<PORTRAIT,TAccessMode>::getHeight() const {
		  return 320;
		}


		/**
		 * Move the display output rectangle
		 * @param rc The display output rectangle
		 */

		template<class TAccessMode>
		inline void ILI9325Orientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {

			this->_accessMode.writeCommand(ili9325::HorizontalRAMPositionStartCmd::Opcode,rc.X);
			this->_accessMode.writeCommand(ili9325::HorizontalRAMPositionEndCmd::Opcode,rc.X+rc.Width-1);

			this->_accessMode.writeCommand(ili9325::VerticalRAMPositionStartCmd::Opcode,rc.Y);
			this->_accessMode.writeCommand(ili9325::VerticalRAMPositionEndCmd::Opcode,rc.Y+rc.Height-1);

			this->_accessMode.writeCommand(ili9325::HorizontalAddressCmd::Opcode,rc.X);
			this->_accessMode.writeCommand(ili9325::VerticalAddressCmd::Opcode,rc.Y);
		}


		/**
		 * Set a vertical scroll position
		 * @param scrollPosition The new scroll position
		 */

		template<class TAccessMode>
		inline void ILI9325Orientation<PORTRAIT,TAccessMode>::setScrollPosition(int16_t scrollPosition) {

			if(scrollPosition<0)
        scrollPosition+=320;
      else if(scrollPosition>319)
        scrollPosition-=320;

      // write to the register

      this->_accessMode.writeCommand(ili9325::GateScanControlScrollCmd::Opcode,scrollPosition);
		}
	}
}

