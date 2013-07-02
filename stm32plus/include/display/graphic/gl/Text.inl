/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include <stdlib.h>
#include "string/StringUtil.h"


namespace stm32plus {
	namespace display {


		/**
		 * Set the flag that controls whether the background is filled or not
		 * @param fontFilledBackground true to fill non-character space with the background colour
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::setFontFilledBackground(bool fontFilledBackground) {
			_fontFilledBackground=fontFilledBackground;
		}

		/**
		 * Write a null terminated string of characters to the display.
		 * Returns the size of the string
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline Size GraphicsLibrary<TDevice,TDeviceAccessMode>::writeString(const Point& p,const Font& font,const char *str) {

			const char *ptr;
			Point pos(p);
			const FontChar *fc;
			int16_t width;
			Size s;

			s.Height=font.getHeight();
			s.Width=0;

			// print each character in turn

			for(ptr=str;*ptr;ptr++) {

				font.getCharacter((uint8_t)*ptr,fc);

				if(_fontFilledBackground)
					writeCharacterFill(pos,font,*fc);
				else
					writeCharacterNoFill(pos,font,*fc);

				width=fc->PixelWidth+font.getCharacterSpacing();
				pos.X+=width;
				s.Width+=width;
			}

			return s;
		}

		/**
		 * Write a single character
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::writeCharacterFill(const Point& p,const Font& font,const FontChar& fc) {

			const uint32_t *bitbandLocation;
			uint16_t numPixels;

			// move to the rectangle that encloses the character

			this->moveTo(
				Rectangle(
						p.X,
						p.Y,
						fc.PixelWidth,
						font.getHeight()
				)
			);

			// get the first value and the corresponding position of that value in the bitband region

			bitbandLocation=reinterpret_cast<uint32_t *> (SRAM_BB_BASE | ((reinterpret_cast<uint32_t>(fc.Data)-SRAM_BASE) << 5));

			// go from top to bottom, left to right

			this->beginWriting();

			numPixels=font.getHeight()*fc.PixelWidth;
			while(numPixels--) {

				// if there's a value at this position, write a pixel

				if(*bitbandLocation++)
					this->writePixel(_foreground);
				else
					this->writePixel(_background);
			}
		}


		/**
		 * Write a single character withoug filling the background. Slower than filling the background due
		 * to the movement calculations
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::writeCharacterNoFill(const Point& p,const Font& font,const FontChar& fc) {

			uint8_t needToMove;
      uint32_t numBits,*bitbandLocation;
      int16_t baseY,x,y;

      // get the font character

      bitbandLocation=reinterpret_cast<uint32_t *> (SRAM_BB_BASE | ((reinterpret_cast<uint32_t>(fc.Data)-SRAM_BASE) << 5));

      baseY=p.Y;

      x=0;
      y=baseY;

      // follow the data

      numBits=static_cast<uint32_t>(font.getHeight()) * static_cast<uint32_t>(fc.PixelWidth);
      needToMove=false;

      while(numBits--) {
        if(*bitbandLocation++) {

        	// if we've been skipping then move

          if(needToMove) {
          	this->moveTo(Rectangle(x+p.X,y,fc.PixelWidth-x,font.getHeight()-(y-baseY)));
            needToMove=false;
            this->beginWriting();
          }

          // write out the pixel

          this->writePixel(_foreground);

          // the position is automatically incremented horizontally but if we run out
          // of width then we'll have to move the position next time

          if(++x==fc.PixelWidth) {
            y++;
            x=0;
            needToMove=true;
          }
        } else {
          // nothing at this position, update x,y

          if(++x==fc.PixelWidth) {
            // hit the width, bump y and reset x

            x=0;
            y++;
          }
          needToMove=true;
        }
      }
		}


		/**
		 * Get the font currently selected for use in stream operations
		 * @return The font pointer, or NULL. You do not own this pointer
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline const Font *GraphicsLibrary<TDevice,TDeviceAccessMode>::getStreamSelectedFont() const {
			return _streamSelectedFont;
		}


		/**
		 * Measure the rectangle required to display the given null terminated string.
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline Size GraphicsLibrary<TDevice,TDeviceAccessMode>::measureString(const Font& font,const char *str) const {

			Size size;
			uint8_t c;
			const FontChar *fc;

			size.Height=font.getHeight();
			size.Width=0;

			while((c=*str++) != 0) {
				font.getCharacter(c,fc);
				size.Width+=fc->PixelWidth+font.getCharacterSpacing();
			}

			return size;
		}


		/**
		 * Write a string
		 * @param str
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(const char *str) {

			_streamSelectedPoint.X+=writeString(_streamSelectedPoint,*_streamSelectedFont,str).Width;
			return *this;
		}


		/**
		 * Write a character
		 * @param c
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(char c) {

			const FontChar *fc;

			_streamSelectedFont->getCharacter((uint8_t)c,fc);

			if(_fontFilledBackground)
				writeCharacterFill(_streamSelectedPoint,*_streamSelectedFont,*fc);
			else
				writeCharacterNoFill(_streamSelectedPoint,*_streamSelectedFont,*fc);

			_streamSelectedPoint.X+=fc->PixelWidth+_streamSelectedFont->getCharacterSpacing();
			return *this;
		}


		/**
		 * Write a 32 bit signed integer
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(int32_t val) {

			char buf[15];
			StringUtil::itoa(val,buf,10);
			_streamSelectedPoint.X+=writeString(_streamSelectedPoint,*_streamSelectedFont,buf).Width;

			return *this;
		}

		/**
		 * Write a 32 bit unsigned integer
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(uint32_t val) {

			char buf[15];
			StringUtil::modp_uitoa10(val,buf);
			_streamSelectedPoint.X+=writeString(_streamSelectedPoint,*_streamSelectedFont,buf).Width;

			return *this;
		}

		/**
		 * Write a 16 bit signed integer
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(int16_t val) {
			return operator<<((int32_t)val);
		}

		/**
		 * Write a 16 bit unsigned integer
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(uint16_t val) {
			return operator<<((int32_t)val);
		}


		/**
		 * Change text output position to the point
		 * @param p The point to move to
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(const Point& p) {
			_streamSelectedPoint=p;
			return *this;
		}


		/**
		 * Change the font used for text output
		 * @param f A reference to the font object
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(const Font& f) {
			_streamSelectedFont=&f;
			return *this;
		}


		/**
		 * Write a double precision value with 5 fractional digits
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(double val) {
			operator<<(DoublePrecision(val,DoublePrecision::MAX_DOUBLE_FRACTION_DIGITS));
			return *this;
		}


		/**
		 * Write a double precision value with user supplied fractional digits
		 * @param val
		 * @return
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline GraphicsLibrary<TDevice,TDeviceAccessMode>& GraphicsLibrary<TDevice,TDeviceAccessMode>::operator<<(const DoublePrecision& val) {

			char str[25];

			StringUtil::modp_dtoa(val.Value,val.Precision,str);
			_streamSelectedPoint.X+=writeString(_streamSelectedPoint,*_streamSelectedFont,str).Width;
			return *this;
		}
	}
}
