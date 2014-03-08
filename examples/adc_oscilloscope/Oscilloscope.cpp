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

  // create the main objects

  _display.reset(new Display);
  createAdc();

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
