/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor. No need to clean up the objects allocated on the heap because
 * they exist for the life of the application.
 */

Display::Display() {

  // FSMC timings for this LCD

  Fsmc8080LcdTiming fsmcTiming(2,4);

  // reset is on PE1 and RS (D/CX) is on PE3
  // no need to keep these objects in scope after they've initialised
  // the GPIO pins for us.

  GpioE<DefaultDigitalOutputFeature<1>,
        DefaultFsmcAlternateFunctionFeature<3> > pe;

  // create the access mode

  _accessMode=new LcdAccessMode(fsmcTiming,19,pe[1]);

  // create the panel

  _gl=new LcdPanel(*_accessMode);

  // create the font that we'll use for the GUI

  _font=new Font_VOLTER__28GOLDFISH_299;
  *_gl << *_font;

  // apply gamma settings for this panel

  ST7783Gamma gamma(0,0x0107,0,0x0203,0x0402,0,0x0207,0,0x0203,0x0403);
  _gl->applyGamma(gamma);

  // redraw all the display

  drawAll();

  // create a PWM backlight signal on PD13 and fade up to 100% in 4ms steps

  _backlight=new DefaultBacklight;
  _backlight->fadeTo(100,4);
}


/*
 * Redraw the whole screen
 */

void Display::drawAll() {

  // clear the screen

  _gl->setBackground(ColourNames::BLACK);
  _gl->clearScreen();

  _controls.drawAll(*this);
}
