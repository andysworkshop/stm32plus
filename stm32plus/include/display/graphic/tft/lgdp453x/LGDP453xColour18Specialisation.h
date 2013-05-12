/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/PanelConfiguration.h"
#include "display/graphic/tft/lgdp453x/commands/GRAMStartWritingCmd.h"
#include "display/graphic/tft/lgdp453x/commands/EntryModeCmd.h"


namespace stm32plus {
	namespace display {


		/**
		 * Template class holding the specialisation of LGDP453xColour for 18-bit colours
		 * @tparam TAccessMode The access mode class (e.g. FSMC)
		 */

		template<class TAccessMode>
		class LGDP453xColour<COLOURS_18BIT,TAccessMode> {

			private:
				TAccessMode& _accessMode;

			protected:
				LGDP453xColour(TAccessMode& accessMode);

				constexpr uint16_t getColourEntryMode() const;

				typedef uint32_t tCOLOUR;

				struct UnpackedColour {
				  uint16_t first,second;
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

		template<class TAccessMode>
		inline LGDP453xColour<COLOURS_18BIT,TAccessMode>::LGDP453xColour(TAccessMode& accessMode)
			: _accessMode(accessMode) {
		}


		/**
		 * Get the register setting for 16-bit colours
		 * @return 16-bit entry mode register setting
		 */

		template<class TAccessMode>
		constexpr inline uint16_t LGDP453xColour<COLOURS_18BIT,TAccessMode>::getColourEntryMode() const {
			return lgdp453x::EntryModeCmd::DFM | lgdp453x::EntryModeCmd::TRI;
		}


		/**
		 * Unpack the colour from #rrggbb to the interal 6-6-6 format

     * 00000000RRRRRRRR GGGGGGGGBBBBBBBB =>
     * 00000000000000RR RRRRGGGGGGBBBBBB
     *
     * This is the format expected by DFM=1, TRI=1. See datasheet for some fancy pictures. This is a 24-bit to
     * 18-bit conversion. The lower 2 bits each of R,G,B are lost.

		 * @param src #rrggbb
		 * @param dest The unpacked colour structure
		 */

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {

	    dest.first=static_cast<uint16_t>(src >> 22);
      dest.second=static_cast<uint16_t>((src & 0xfc0000) >> 6 | (src & 0xfc00) >> 4 | (src & 0xfc) >> 2);
		}

		/**
		 * Unpack the colour from components to the internal format
		 * @param red
		 * @param green
		 * @param blue
		 * @param dest
		 */

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

			red&=0xfc;
			green&=0xfc;
			blue&=0xfc;

			dest.first=red >> 6;
			dest.second=(((uint16_t)red) << 10) | (((uint16_t)green) << 4) | blue >> 2;
		}


		/**
		 * Write a single pixel to the current output position.
		 * Assumes that the caller has already issued the beginWriting() command.
		 * @param cr The pixel to write
		 */

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::writePixel(const UnpackedColour& cr) const {
			this->_accessMode.writeData(cr.first);
			this->_accessMode.writeData(cr.second);
		}


		/**
		 * Fill a block of pixels with the same colour. This operation will issue the
		 * beginWriting() command for you.
		 * @param numPixels how many
		 * @param cr The unpacked colour to write
		 */

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

			uint16_t first,second;

			this->_accessMode.writeCommand(lgdp453x::GRAMStartWritingCmd::Opcode);

			first=cr.first;
			second=cr.second;

			while(numPixels--) {
				this->_accessMode.writeData(first);
				this->_accessMode.writeData(second);
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

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
		  buffer=new uint8_t[numPixels*4];
		  bytesPerPixel=4;
		}


		/**
		 * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
		 * for transfer.
		 * @param buffer The memory buffer
		 * @param numPixels The number of pixels to transfer from the buffer
		 */

		template<class TAccessMode>
		inline void LGDP453xColour<COLOURS_18BIT,TAccessMode>::rawTransfer(const void *buffer,uint32_t numPixels) const {

			volatile uint16_t *dataAddress=this->_accessMode.getDataAddress();
			const uint16_t *ptr=static_cast<const uint16_t *>(buffer);

			while(numPixels--) {
				*dataAddress=*ptr++;
				*dataAddress=*ptr++;
			}
		}
	}
}
