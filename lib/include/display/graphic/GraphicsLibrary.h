/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

  /**
   * Template implementation of a graphics library. This library inherits from the device implementation
   * that provides the methods that the graphics library needs to access the hardware
   *
   * @tparam TDevice The device driver type that this graphics library is customised for
   * @tparam TDeviceAccessMode The access mode class for the device
   */

  template<class TDevice,typename TDeviceAccessMode>
  class GraphicsLibrary : public TDevice {

    public:
      typedef typename TDevice::UnpackedColour UnpackedColour;    ///< Helper type for the unpacked colour structure
      typedef typename TDevice::tCOLOUR tCOLOUR;                  ///< Helper type for the packed colour type

    protected:

      UnpackedColour _foreground;
      UnpackedColour _background;

      Point _streamSelectedPoint;         // need to keep a copy so rvalue points can be used
      const Font *_streamSelectedFont;    // can keep a ptr, user should not delete font while selected
      bool _fontFilledBackground;         // true to use filled backgrounds for fonts

    protected:
      void plot4EllipsePoints(int16_t cx,int16_t cy,int16_t x,int16_t y);

    public:
      GraphicsLibrary(TDeviceAccessMode& accessMode);

      // colour choices

      void setForeground(tCOLOUR cr);
      void setForeground(uint8_t r,uint8_t g,uint8_t b);

      void setBackground(tCOLOUR cr);
      void setBackground(uint8_t r,uint8_t g,uint8_t b);

      // panel querying

      int16_t getXmax() const;
      int16_t getYmax() const;
      Rectangle getFullScreenRectangle() const;

      // text output methods

      void setFontFilledBackground(bool fontFilledBackground);

      Size writeString(const Point& p,const Font& font,const char *str);
      void writeCharacterFill(const Point& p,const Font& font,const FontChar& fc);
      void writeCharacterNoFill(const Point& p,const Font& font,const FontChar& fc);
      Size measureString(const Font& font,const char *str) const;
      const Font *getStreamSelectedFont() const;

      // text output methods - LZG fonts

      Size writeString(const Point& p,const LzgFont& font,const char *str);
      void writeCharacter(const Point& p,const LzgFont& font,const FontChar& fc);

      // can't do these as a template with specialisation because you can't specialise
      // members in a template class that isn't also fully specialised

      GraphicsLibrary& operator<<(const char *str);
      GraphicsLibrary& operator<<(char c);
      GraphicsLibrary& operator<<(int32_t val);
      GraphicsLibrary& operator<<(uint32_t val);
      GraphicsLibrary& operator<<(int16_t val);
      GraphicsLibrary& operator<<(uint16_t val);
      GraphicsLibrary& operator<<(const DoublePrecision& val);
      GraphicsLibrary& operator<<(double val);
      GraphicsLibrary& operator<<(const Point& p);
      GraphicsLibrary& operator<<(const Font& f);

      // drawing primitives

      void moveToPoint(const Point& pt) const;
      void clearScreen();
      void plotPoint(const Point& p);
      void drawRectangle(const Rectangle& rc);
      void fillRectangle(const Rectangle& rc);
      void clearRectangle(const Rectangle& rc);
      void gradientFillRectangle(const Rectangle& rc,Direction dir,tCOLOUR first,tCOLOUR last);
      void drawEllipse(const Point& center,const Size& size);
      void fillEllipse(const Point& center,const Size& size);
      void drawLine(const Point& p1,const Point& p2);

      // bitmap handling

      template<class TDmaCopierImpl>
      bool drawBitmap(const Rectangle& rc,InputStream& source,DmaLcdWriter<TDmaCopierImpl>& dma,uint32_t priority=DMA_Priority_High);
      bool drawBitmap(const Rectangle& rc,InputStream& source);

      // jpeg handling

      void drawJpeg(const Rectangle& rc,InputStream& source);
    };
  }
}


/*
 * include the template implementations
 */

#include "gl/Fundamentals.inl"
#include "gl/Primitives.inl"
#include "gl/Ellipse.inl"
#include "gl/Rectangle.inl"
#include "gl/Text.inl"
#include "gl/LzgText.inl"
#include "gl/Bitmap.inl"

// the text operations use bitbanding on the f1 and f4. not available on the f0.

#if defined(STM32PLUS_F0)
  #include "gl/f0/Text.inl"
#elif defined(STM32PLUS_F1)
  #include "gl/f1/Text.inl"
#elif defined(STM32PLUS_F4)
  #include "gl/f4/Text.inl"
#endif
