/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Initialiser, create the LCD object. Gamma and fsmc objects need to be in scope but
 * can be lost on the heap as they'll never be referenced again externally.
 */

bool LcdManager::initialise() {

  // reset is on PE1 and RS (D/CX) is on PD11

  GpioE<DefaultDigitalOutputFeature<1> > pe;
  GpioD<DefaultAlternateFunctionFeature<GPIO_AF_FSMC,11> > pd;

  // set up the FSMC on bank 0 with A16 as the RS line

  Fsmc8080LcdTiming fsmcTiming(2,5);
  _accessMode=new LcdAccessMode(fsmcTiming,16,pe[1]);

  // create the LCD and object

  _lcd=new LcdAccess(*_accessMode);

  // create and select the font

  _font=new Font_APPLE8();
  *_lcd << *_font;

  // create the terminal object using the selected font

  _terminal=new TerminalAccess(*_lcd);

  // apply gamma settings

  ILI9325Gamma gamma(0x0006,0x0101,0x0003,0x0106,0x0b02,0x0302,0x0707,0x0007,0x0600,0x020b);
  _lcd->applyGamma(gamma);

  // clear down to black

  _lcd->setBackground(ColourNames::BLACK);
  _lcd->setForeground(ColourNames::WHITE);

  _terminal->clearScreen();

  // lights on at 100% in 4ms steps from zero.

  _backlight=new DefaultBacklight;
  _backlight->fadeTo(100,4);

  return true;
}
