/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


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
