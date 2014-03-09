/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Run the scope
 */

void Oscilloscope::run() {

  // reset the flags

  _halfTime=_fullTime=false;

  // create the main objects

  _display.reset(new Display);

  createAdc();
  createBuffers();

  for(;;);
}


/*
 * Create an appropriate type of ADC for this operating mode
 */

void Oscilloscope::createAdc() {

  if(_display->getOperatingMode()==OperatingModeType::CONTINUOUS)
    _adc.reset(new AdcContinuous);
  else
    _adc.reset(new AdcTriggered);
}


/*
 * Create the sample buffers
 */

void Oscilloscope::createBuffers() {

  // each buffer is 2x the width of the screen

  _channel1.reset(new uint8_t[LcdPanel::LONG_SIDE*2]);
  _channel2.reset(new uint8_t[LcdPanel::LONG_SIDE*2]);
}
