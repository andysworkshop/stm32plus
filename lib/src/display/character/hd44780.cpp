/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/character.h"


namespace stm32plus {
  namespace display {

    /**
     * Constructor: 4 bit I/O for minimal pin usage.
     */

    HD44780::HD44780(GpioPinRef rs,GpioPinRef enable,GpioPinRef d0,GpioPinRef d1,GpioPinRef d2,GpioPinRef d3,
                     uint8_t cols,uint8_t rows,uint8_t dotsize) {

      _rs_pin=rs;
      _enable_pin=enable;

      _data_pins[0]=d0;
      _data_pins[1]=d1;
      _data_pins[2]=d2;
      _data_pins[3]=d3;

      _displayfunction=LCD_1LINE | LCD_5x8DOTS;

      if(rows>1)
        _displayfunction|=LCD_2LINE;

      _numlines=rows;
      _numcols=cols;
      _currline=0;

      // for some 1 line displays you can select a 10 pixel high font

      if(dotsize!=0 && rows==1)
        _displayfunction|=LCD_5x10DOTS;

      // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
      // according to datasheet, we need at least 40ms after power rises above 2.7V
      // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50

      MillisecondTimer::delay(50);

      // Now we pull RS low to begin commands
      _rs_pin.reset();
      _enable_pin.reset();

      // put the LCD into 4 bit mode

      // this is according to the hitachi HD44780 datasheet figure 24, pg 46
      // we start in 8bit mode, try to set 4 bit mode
      write4bits(3);
      MillisecondTimer::delay(45);

      // second try
      write4bits(3);
      MillisecondTimer::delay(45);

      // third go!
      write4bits(3);
      MillisecondTimer::delay(2);

      // set to 8-bit interface
      write4bits(2);

      // finally, set # lines, font size, etc.
      sendCommand(LCD_FUNCTIONSET | _displayfunction);

      // turn the display on with no cursor or blinking default
      _displaycontrol=LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
      setDisplay(DISPLAY_ON);

      // clear it off
      clear();

      // Initialize to default text direction
      _displaymode=LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

      // set the entry mode
      sendCommand(LCD_ENTRYMODESET | _displaymode);
    }

    /**
     * clear display, set cursor position to zero
     */

    void HD44780::clear() {
      sendCommand(LCD_CLEARDISPLAY);
      MillisecondTimer::delay(2);
    }

    /**
     * Move the cursor to the given co-ords
     * @param[in] col column address
     * @param[in] row row address
     */

    void HD44780::moveCursor(uint8_t col,uint8_t row) {
      const int row_offsets[]= { 0x00,0x40,0x14,0x54 };

      if(row>_numlines)
        row=_numlines-1; // we count rows starting w/0

      sendCommand(LCD_SETDDRAMADDR | (col+row_offsets[row]));
    }

    /**
     * Change the display state
     * @param[in] newState The new state of the display.
     */

    void HD44780::setDisplay(DisplayState newState) {

      if(newState==DISPLAY_ON)
        _displaycontrol|=LCD_DISPLAYON;
      else
        _displaycontrol&=~LCD_DISPLAYON;

      sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    /**
     * Set the cursor state
     * @param[in] newState The new cursor state.
     */

    void HD44780::setCursor(CursorState newState) {

      if(newState==CURSOR_ON)
        _displaycontrol|=LCD_CURSORON;
      else
        _displaycontrol&=~LCD_CURSORON;

      sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    /**
     * Change the blink state
     * @param[in] newState The new display state.
     */

    void HD44780::setBlink(BlinkState newState) {

      if(newState==BLINK_ON)
        _displaycontrol|=LCD_BLINKON;
      else
        _displaycontrol&=~LCD_BLINKON;

      sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    /**
     * Scroll the display
     * @param[in] direction The new scroll direction.
     */

    void HD44780::scrollDisplay(ScrollDirection direction) {
      if(direction==SCROLL_LEFT)
        sendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
      else
        sendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    }

    /**
     * Set the text output direction
     * @param[in] direction The new text direction.
     */

    void HD44780::setTextDirection(TextDirection direction) {

      if(direction==TEXT_DIRECTION_LR)
        _displaymode|=LCD_ENTRYLEFT;
      else
        _displaymode&=~LCD_ENTRYLEFT;

      sendCommand(LCD_ENTRYMODESET | _displaymode);
    }

    /**
     * Set the auto-scroll state
     * @param[in] newState The new auto-scroll state
     */

    void HD44780::setAutoScroll(AutoScrollState newState) {

      if(newState==AUTO_SCROLL_ON)
        _displaymode|=LCD_ENTRYSHIFTINCREMENT;
      else
        _displaymode&=~LCD_ENTRYSHIFTINCREMENT;

      sendCommand(LCD_ENTRYMODESET | _displaymode);
    }

    /**
     * Allows us to fill the first 8 CGRAM locations with custom characters
     * @param[in] location The custom character code (0..7)
     * @param[in] charmap The 8 bytes that define this character.
     */

    void HD44780::createCustomChar(uint8_t location,uint8_t charmap[]) {

      location&=7; // we only have 8 locations 0-7
      sendCommand(LCD_SETCGRAMADDR|(location<<3));

      for(int i=0;i<8;i++)
        write(charmap[i]);
    }

    /**
     * send a command byte
     * @param[in] value The command byte to send.
     */

    void HD44780::sendCommand(uint8_t value) const {
      send(value,false);
    }

    /**
     * Write a single character at the current cursor position
     * @param[in] value the character to write
     */

    void HD44780::write(uint8_t value) const {
      send(value,true);
    }

    /*
     * write either command or data, with automatic 4/8-bit selection
     */

    void HD44780::send(uint8_t value,uint8_t mode) const {

      // set mode
      _rs_pin.setState(mode);

      write4bits(value>>4);
      write4bits(value);
    }

    /*
     * Enable pulse
     */

    void HD44780::pulseEnable(void) const {

      _enable_pin.reset();
      _enable_pin.set();
      MillisecondTimer::delay(1); // (anything>450ns)

      _enable_pin.reset();
      MillisecondTimer::delay(1); // commands need > 37us to settle
    }

    /*
     * write 4 bits
     */

    void HD44780::write4bits(uint8_t value) const {

      for(int i=0;i<4;i++)
        _data_pins[i].setState((value >> i) & 1);

      pulseEnable();
    }


    /**
     * Get the width of the display in characters
     * @return The width.
     */

    uint16_t HD44780::getWidth() const {
      return _numcols;
    }


    /**
     * Get the height of the display in characters
     * @return The height.
     */

    uint16_t HD44780::getHeight() const {
      return _numlines;
    }
  }
}
