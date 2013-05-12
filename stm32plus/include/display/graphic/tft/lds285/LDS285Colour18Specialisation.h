/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/lds285/commands/AllCommands.h"


namespace stm32plus {
	namespace display {


		/**
		 * Template class holding the specialisation of LDS285Colour for 18-bit colours
		 * @tparam TAccessMode The access mode class (e.g. FSMC)
		 */

		template<class TAccessMode,class TPanelTraits>
		class LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits> {

			private:
				TAccessMode& _accessMode;

			protected:
				LDS285Colour(TAccessMode& accessMode);

				constexpr uint16_t getInterfacePixelFormat() const;

				typedef uint32_t tCOLOUR;

				struct UnpackedColour {
				  uint8_t r,g,b;
				};

		  public:
				void unpackColour(tCOLOUR src,UnpackedColour& dest) const;
				void unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const;

				void writePixel(const UnpackedColour& cr) const;
				void fillPixels(uint32_t numPixels,const UnpackedColour& cr) const;

				void allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const;
				void rawTransfer(const void *data,uint32_t numPixels) const;
		};


		/**
		 * Constructor
		 */

		template<class TAccessMode,class TPanelTraits>
		inline LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::LDS285Colour(TAccessMode& accessMode)
			: _accessMode(accessMode) {
		}


		/**
		 * Get the register setting for 16-bit colours
		 * @return 16-bit interface pixel format register setting
		 */

		template<class TAccessMode,class TPanelTraits>
		constexpr inline uint16_t LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::getInterfacePixelFormat() const {
			return 0x66;
		}


		/**
		 * Unpack the colour from rrggbb to the interal 6-6-6 format

     * 00000000RRRRRRRRGGGGGGGGBBBBBBBB ->
     * 00000000RRRRRR00GGGGGG00BBBBBB00

		 * @param src rrggbb
		 * @param dest The unpacked colour structure
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {
			dest.r=(src >> 16) & 0xfc;
			dest.g=(src >> 8) & 0xfc;
			dest.b=src & 0xfc;
		}

		/**
		 * Unpack the colour from components to the internal format
		 * @param red
		 * @param green
		 * @param blue
		 * @param dest
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {
			dest.r=red & 0xfc;
			dest.g=green & 0xfc;
			dest.b=blue & 0xfc;
		}

		/**
		 * Write a single pixel to the current output position.
		 * Assumes that the caller has already issued the beginWriting() command.
		 * @param cr The pixel to write
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::writePixel(const UnpackedColour& cr) const {
			this->_accessMode.writeData(cr.r);
			this->_accessMode.writeData(cr.g);
			this->_accessMode.writeData(cr.b);
		}


		/**
		 * Fill a block of pixels with the same colour. This operation will issue the
		 * beginWriting() command for you.
		 * @param numPixels how many
		 * @param cr The unpacked colour to write
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

			uint8_t b,g,r;

			this->_accessMode.writeCommand(lds285::MEMORY_WRITE);

			r=cr.r;
			g=cr.g;
			b=cr.b;

			while(numPixels--) {
				this->_accessMode.writeData(r);
				this->_accessMode.writeData(g);
				this->_accessMode.writeData(b);
			}
		}


		/**
		 * Allocate a buffer for pixel data. You supply the number of pixels and this allocates the buffer as a uint8_t[].
		 * Allocated buffers should be freed with delete[]
		 *
		 * @param numPixels The number of pixels to allocate
		 * @param buffer The output buffer
		 * @param bytesPerPixel Output the number of bytes per pixel
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
		  buffer=new uint8_t[numPixels*3];
		  bytesPerPixel=3;
		}


		/**
		 * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
		 * for transfer.
		 * @param buffer The memory buffer
		 * @param numPixels The number of pixels to transfer from the buffer
		 */

		template<class TAccessMode,class TPanelTraits>
		inline void LDS285Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::rawTransfer(const void *buffer,uint32_t numPixels) const {
			this->_accessMode.rawTransfer(buffer,numPixels*3);
		}
	}
}
