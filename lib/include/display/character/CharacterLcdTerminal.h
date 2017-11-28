/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * CharacterLcdTerminal terminal(lcd);
     */

    template<class TImpl>
    class CharacterLcdTerminal {

      protected:
        char *_framebuffer;
        Size _terminalSize;
        Point _cursor;
        TImpl& _lcd;

      public:
        CharacterLcdTerminal(TImpl& lcd);
        ~CharacterLcdTerminal();

        void writeCharacter(int16_t x,int16_t y,char c);
        void writeCharacter(char c);
        void writeString(const char *str);

        void incrementY();
        void clear();
        void clearLine();

        CharacterLcdTerminal& operator<<(const char *str);
        CharacterLcdTerminal& operator<<(char c);
        CharacterLcdTerminal& operator<<(int32_t val);
        CharacterLcdTerminal& operator<<(uint32_t val);
        CharacterLcdTerminal& operator<<(int16_t val);
        CharacterLcdTerminal& operator<<(uint16_t val);
        CharacterLcdTerminal& operator<<(const DoublePrecision& val);
        CharacterLcdTerminal& operator<<(double val);
    };


    /**
     * Constructor
     */

    template<class TImpl>
    inline CharacterLcdTerminal<TImpl>::CharacterLcdTerminal(TImpl& lcd)
      : _lcd(lcd) {

      _cursor.X=_cursor.Y=0;

      _terminalSize.Width=lcd.getWidth();
      _terminalSize.Height=lcd.getHeight();

      _framebuffer=new char[_terminalSize.Width*_terminalSize.Height];
      memset(_framebuffer,' ',_terminalSize.Width*_terminalSize.Height);
    }


    /**
     * Destructor
     */

    template<class TImpl>
    inline CharacterLcdTerminal<TImpl>::~CharacterLcdTerminal() {
      delete [] _framebuffer;
    }


    /**
     * Write a character
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::writeCharacter(int16_t x,int16_t y,char c) {

      // write the character to the display

      _lcd.moveCursor(x,y);
      _lcd.write(c);

      // and the framebuffer

      _framebuffer[x+y*_terminalSize.Width]=c;
    }


    /**
     * Write a string to the terminal
     * @param str
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::writeString(const char *str) {
      const char *ptr;

      for(ptr=str;*ptr;ptr++) {
        if(*ptr == '\n') {
          incrementY();
          _cursor.X=0;
        } else if(*ptr=='\r') {
          _cursor.X=0;
        } else {
          writeCharacter(_cursor.X,_cursor.Y,*ptr);

          if(++_cursor.X >= _terminalSize.Width) {
            _cursor.X=0;
            incrementY();
          }
        }
      }
    }


    /**
     * Write a character.
     *
     * @param[in] c The character to write.
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::writeCharacter(char c) {
      char buf[2];

      buf[0]=c;
      buf[1]='\0';

      writeString(buf);
    }


    /**
     * Clear the terminal
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::clear() {

      _cursor.X=0;
      _cursor.Y=0;
      memset(_framebuffer,' ',_terminalSize.Width*_terminalSize.Height);
      _lcd.clear();
    }


    /**
     * Clear just the current line
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::clearLine() {

      int16_t i;

      // move to start of line and clear the framebuffer line

      _cursor.X=0;
      memset(&_framebuffer[_cursor.Y*_terminalSize.Width],' ',_terminalSize.Width);
      _lcd.moveCursor(0,_cursor.Y);

      // write out spaces to clear the line

      for(i=0;i<_terminalSize.Width;i++)
        _lcd.write(' ');
    }


    /**
     * Increment the Y co-ordinate. Incrementing past the physical size of the display
     * scrolls the display vertically and keeps the y-co-ordinate at the bottom. The act
     * of scrolling will refresh the entire display.
     */

    template<class TImpl>
    inline void CharacterLcdTerminal<TImpl>::incrementY() {

      int16_t x,y;
      char *ptr;

      // easy if not yet at the bottom of the display

      if(_cursor.Y!=_terminalSize.Height-1)
        _cursor.Y++;
      else {

        // now we 'scroll' the framebuffer and clear out the last line

        memmove(_framebuffer,_framebuffer+_terminalSize.Width,_terminalSize.Width*(_terminalSize.Height-1));
        memset(_framebuffer+_terminalSize.Width*(_terminalSize.Height-1),' ',_terminalSize.Width);

        // refresh the whole frame

        ptr=_framebuffer;
        for(y=0;y<_terminalSize.Height;y++) {
          _lcd.moveCursor(0,y);
          for(x=0;x<_terminalSize.Width;x++) {
            _lcd.write(*ptr++);
          }
        }
      }
    }


    /**
     * Write a string using the stream operator
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(const char *str) {

      writeString(str);
      return *this;
    }

    /**
     * Write a character
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(char c) {

      writeCharacter(c);
      return *this;
    }

    /**
     * Write a 16 bit signed int
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(int16_t val) {

      return operator<<((int32_t)val);
    }

    /**
     * Write a 16 bit unsigned int
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(uint16_t val) {

      return operator<<((int32_t)val);
    }

    /**
     * Write a 32 bit signed int
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(int32_t val) {

      char buf[15];
      StringUtil::itoa(val,buf,10);
      writeString(buf);

      return *this;
    }

    /**
     * Write a 32 bit unsigned int
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(uint32_t val) {

      char buf[15];
      StringUtil::modp_uitoa10(val,buf);
      writeString(buf);

      return *this;
    }

    /**
     * Write a double precision value with 5 fractional digits
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(double val) {

      return operator<<(DoublePrecision(val, DoublePrecision::MAX_DOUBLE_FRACTION_DIGITS));
    }

    /**
     * Write a double precision value with customisable fractional digits
     */

    template<class TImpl>
    CharacterLcdTerminal<TImpl>& CharacterLcdTerminal<TImpl>::operator<<(const DoublePrecision& val) {

      char buf[25];

      StringUtil::modp_dtoa(val.Value,val.Precision,buf);
      writeString(buf);
      return *this;
    }
  }
}

