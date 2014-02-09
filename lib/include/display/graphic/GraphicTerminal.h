/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Base class to manage the display as a character terminal. Hardware scrolling is supported
     * as an option. When not supported the terminal will clear and restart at the top when the
     * end is reached.
     *
     * The font must be fixed width or wierd stuff will happen.
     *
     * This class implements output stream so it can be used as a sink for data sourced from other
     * places. For example, see ConnectedInputOutputStream for a plumbing class that can be used to join an
     * input stream to an output stream.
     *
     * Normally you would use the default terminal types created for you in the TftInterfaces.h
     * file for the display you are using.
     *
     * @tparam TGraphicsLibrary The complete type of the graphics library being used.
     * @tparam THardwareScrolling Set to true if scrolling supported.
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    class GraphicTerminal : public OutputStream {

      protected:
        TGraphicsLibrary& _gl;
        const Font *_font;
        bool _autoLineFeed;

        Size _terminalSize;
        Size _fontSize;
        Point _cursor;
        uint8_t _smoothScrollStep;
        int16_t _scrollPosition;

      protected:
        void calcTerminalSize();
        void incrementY();
        void scroll();

      public:
        enum {
          NO_SMOOTH_SCROLLING = 0,
          DEFAULT_SMOOTH_SCROLL_STEP = 5
        };

      public:
        GraphicTerminal(
            TGraphicsLibrary& gl,
            const Font *font=nullptr,
            bool autoLineFeed=false,
            uint8_t smoothScrollStep=DEFAULT_SMOOTH_SCROLL_STEP);

        virtual ~GraphicTerminal() {}

        void writeCharacter(char c);
        void writeString(const char *str);

        GraphicTerminal& operator<<(const char *str);
        GraphicTerminal& operator<<(char c);
        GraphicTerminal& operator<<(int32_t val);
        GraphicTerminal& operator<<(uint32_t val);
        GraphicTerminal& operator<<(int16_t val);
        GraphicTerminal& operator<<(uint16_t val);
        GraphicTerminal& operator<<(const DoublePrecision& val);
        GraphicTerminal& operator<<(double val);

        void clearScreen();
        void clearLine();

        // overrides from OutputStream

        virtual bool write(uint8_t c) override;
        virtual bool write(const void *buffer,uint32_t size) override;

        virtual bool close() override { return true; }
        virtual bool flush() override { return true; }
    };


    /**
     * Constructor. You probably want to call clearScreen before you get going.
     * The font will default to the one selected for stream IO if none is supplied.
     *
     * @param gl The graphics library (LCD implementation class) to use
     * @param font The font to use, or nullptr to get it from the graphics library selection
     * @param autoLineFeed true to add a line feed when a carriage return is received.
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::GraphicTerminal(
        TGraphicsLibrary& gl,
        const Font *font,
        bool autoLineFeed,
        uint8_t smoothScrollStep)
      : _gl(gl),
        _font(font==nullptr ? gl.getStreamSelectedFont() : font),
        _autoLineFeed(autoLineFeed),
        _smoothScrollStep(smoothScrollStep),
        _scrollPosition(0) {

      calcTerminalSize();
    }


    /**
     * Calculate the terminal size, in characters.
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::calcTerminalSize() {

      const FontChar *fc;

      // need to know the width of the characters. They're all the same so measure a space.

      _font->getCharacter(static_cast<uint8_t>(' '),fc);

      _fontSize.Height=_font->getHeight();
      _fontSize.Width=fc->PixelWidth;

      // height is rounded down if the fixed lines don't sum to a multiple of the font height

      _terminalSize.Width=_gl.getWidth()/fc->PixelWidth;
      _terminalSize.Height=_gl.getHeight()/_font->getHeight();
    }


    /**
     * Clear the screen
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::clearScreen() {

      // clear the display

      _gl.clearScreen();

      _cursor.X=0;
      _cursor.Y=0;

      // reset the scroll position if hardware scrolling is supported

      if(THardwareScrolling) {
        _scrollPosition=0;
        _gl.setScrollPosition(0);
      }
    }


    /**
     * Clear just the current line
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::clearLine() {

      Rectangle rc;

      rc.X=0;
      rc.Y=(_cursor.Y % _terminalSize.Height)*_fontSize.Height;
      rc.Height=_fontSize.Height;
      rc.Width=_terminalSize.Width*_fontSize.Width;

      _gl.clearRectangle(rc);

      _cursor.X=0;
    }


    /**
     * Write a line to the display
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::writeString(const char *str) {

      const char *ptr;
      for(ptr=str;*ptr;writeCharacter(*ptr++));
    }


    /**
     * Write a character to the display
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::writeCharacter(char c) {

      char buffer[2];
      Point p;

      if(c=='\n') {

        incrementY();
        _cursor.X=0;

      } else if(c=='\r') {

        if(_autoLineFeed)
          incrementY();

        _cursor.X=0;
      } else {

        // scale up the x,y character co-ords to pixel co-ords

        p.X=_cursor.X*_fontSize.Width;
        p.Y=(_cursor.Y % _terminalSize.Height)*_fontSize.Height;

        // create a string

        buffer[0]=c;
        buffer[1]='\0';

        // move the cursor and write

        _gl.writeString(p,*_font,buffer);

        if(++_cursor.X >= _terminalSize.Width) {
          _cursor.X=0;
          incrementY();
        }
      }
    }


    /**
     * Increment the row and scroll if we have hit the bottom. If the LCD implementation supports
     * hardware scrolling then we will use it, otherwise we just clear the display and start again
     * at the top left.
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::incrementY() {

      _cursor.Y++;

      if(THardwareScrolling) {
        if(_cursor.Y>=_terminalSize.Height)
          scroll();
      }
      else if(_cursor.Y>=_terminalSize.Height)
        clearScreen();
    }


    /**
     * Scroll the display by one line
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    void GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::scroll() {

      int i;
      Rectangle rc;
      int16_t screenHeight;

      screenHeight=_gl.getHeight();

      rc.Width=_gl.getWidth();
      rc.X=0;

      // smooth scroll

      rc.Height=1;

      for(i=0;i<_fontSize.Height;i++) {

        _scrollPosition++;

        if(_scrollPosition==screenHeight)
          _scrollPosition=0;

        _gl.setScrollPosition(_scrollPosition);

        rc.Y=_scrollPosition>0 ? _scrollPosition-1 : screenHeight-1;
        _gl.clearRectangle(rc);

        MillisecondTimer::delay(_smoothScrollStep);
      }

      // don't allow the cursor to run away and overflow

      if(_cursor.Y==_terminalSize.Height*2)
        _cursor.Y=_terminalSize.Height;
    }

    /**
     * Write a string using the stream operator
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(const char *str) {

      writeString(str);
      return *this;
    }

    /**
     * Write a character
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(char c) {

      writeCharacter(c);
      return *this;
    }

    /**
     * Write a 16 bit signed int
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(int16_t val) {

      return operator<<((int32_t)val);
    }

    /**
     * Write a 16 bit unsigned int
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(uint16_t val) {

      return operator<<((int32_t)val);
    }

    /**
     * Write a 32 bit signed int
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(int32_t val) {

      char buf[15];
      StringUtil::itoa(val,buf,10);
      writeString(buf);

      return *this;
    }

    /**
     * Write a 32 bit unsigned int
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(uint32_t val) {

      char buf[15];
      StringUtil::modp_uitoa10(val,buf);
      writeString(buf);

      return *this;
    }

    /**
     * Write a double precision value with 5 fractional digits
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(double val) {

      return operator<<(DoublePrecision(val,DoublePrecision::MAX_DOUBLE_FRACTION_DIGITS));
    }

    /**
     * Write a double precision value with customisable fractional digits
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline GraphicTerminal<TGraphicsLibrary,THardwareScrolling>& GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::operator<<(const DoublePrecision& val) {

      char buf[25];

      StringUtil::modp_dtoa(val.Value,val.Precision,buf);
      writeString(buf);
      return *this;
    }


    /**
     * Write a single byte
     * @param c The byte to write
     * @return always true
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline bool GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::write(uint8_t c) {
      return write(&c,1);
    }


    /**
     * Write many bytes. Each byte must be interpretable as a character to from the selected
     * font for this to make any sense. Random binary input will result in odd things.
     * @param[in] buffer The buffer of bytes
     * @param[in] size The number of bytes to write
     * @return always true
     */

    template<class TGraphicsLibrary,bool THardwareScrolling>
    inline bool GraphicTerminal<TGraphicsLibrary,THardwareScrolling>::write(const void *buffer,uint32_t size) {

      const char *ptr=reinterpret_cast<const char *>(buffer);

      while(size--)
        writeCharacter(*ptr++);

      return true;
    }
  }
}
