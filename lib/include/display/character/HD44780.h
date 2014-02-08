/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * HD44780 compatible displays. This class is derived from the LiquidCrystal class
     * included with the Arduino (AVR) libraries.
     *
     *
     *
     */

    class HD44780 : public CharacterLcd<HD44780> {

      protected:
        GpioPinRef _rs_pin;         // LOW: command.  HIGH: character.
        GpioPinRef _enable_pin;     // activated by a HIGH pulse.
        GpioPinRef _data_pins[4];

        uint8_t _displayfunction;
        uint8_t _displaycontrol;
        uint8_t _displaymode;

        uint8_t _numlines,_currline,_numcols;

        enum {
          LCD_CLEARDISPLAY=0x01,
          LCD_RETURNHOME=0x02,
          LCD_ENTRYMODESET=0x04,
          LCD_DISPLAYCONTROL=0x08,
          LCD_CURSORSHIFT=0x10,
          LCD_FUNCTIONSET=0x20,
          LCD_SETCGRAMADDR=0x40,
          LCD_SETDDRAMADDR=0x80,

          // flags for display entry mode
          LCD_ENTRYRIGHT=0x00,
          LCD_ENTRYLEFT=0x02,
          LCD_ENTRYSHIFTINCREMENT=0x01,
          LCD_ENTRYSHIFTDECREMENT=0x00,

          // flags for display on/off control
          LCD_DISPLAYON=0x04,
          LCD_DISPLAYOFF=0x00,
          LCD_CURSORON=0x02,
          LCD_CURSOROFF=0x00,
          LCD_BLINKON=0x01,
          LCD_BLINKOFF=0x00,

          // flags for display/cursor shift
          LCD_DISPLAYMOVE=0x08,
          LCD_CURSORMOVE=0x00,
          LCD_MOVERIGHT=0x04,
          LCD_MOVELEFT=0x00,

          // flags for function set
          LCD_2LINE=0x08,
          LCD_1LINE=0x00,
          LCD_5x10DOTS=0x04,
          LCD_5x8DOTS=0x00
        };

      protected:
        void send(uint8_t,uint8_t) const;
        void write4bits(uint8_t) const;
        void pulseEnable() const;

      public:
        HD44780(GpioPinRef rs,GpioPinRef enable,GpioPinRef d0,GpioPinRef d1,GpioPinRef d2,GpioPinRef d3,uint8_t cols,uint8_t rows,uint8_t charsize=LCD_5x8DOTS);

        void createCustomChar(uint8_t,uint8_t[]);
        void sendCommand(uint8_t) const;

        void clear();
        void moveCursor(uint8_t x,uint8_t y);
        void write(uint8_t c) const;

        void setDisplay(DisplayState newState);
        void setBlink(BlinkState newState);
        void setCursor(CursorState newState);
        void scrollDisplay(ScrollDirection direction);
        void setTextDirection(TextDirection direction);
        void setAutoScroll(AutoScrollState newState);

        uint16_t getWidth() const;
        uint16_t getHeight() const;
    };
  }
}

