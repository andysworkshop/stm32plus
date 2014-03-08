/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Types that define the LCD we'll use and the access mode that we'll use to
 * control it. You can change these to match any other display that is supported
 * by the library.
 */

typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
typedef ST7783_Landscape_64K<LcdAccessMode> LcdPanel;


/*
 * Display class, manages interaction with the LCD
 */

class Display {

  protected:
    Controls _controls;

    LcdAccessMode *_accessMode;
    LcdPanel *_gl;
    Font *_font;
    DefaultBacklight *_backlight;

  public:
    Display();

    void drawAll();

    OperatingModeType getOperatingMode() const;
    LcdPanel& getGl() const;
};


/*
 * Get the current operating mode
 */

inline OperatingModeType Display::getOperatingMode() const {
  return _controls.getOperatingMode();
}


/*
 * Get a reference to the graphics library
 */

inline LcdPanel& Display::getGl() const {
  return *_gl;
}
