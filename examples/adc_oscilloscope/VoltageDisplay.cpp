/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

VoltageDisplay::VoltageDisplay() {
  _voltage=388;
}


/*
 * Draw the voltage. Voltages are scaled *10 for storage as an integer e.g 3.14 is stored as 314.
 * The display is always 2 decimal places.
 */

void VoltageDisplay::drawVoltage(LcdPanel& gl,const Point& p) {

  char buffer[10];
  uint32_t v;

  buffer[0]=(_voltage/100)+'0';
  v=_voltage-(100*(_voltage/100));

  buffer[1]='.';

  buffer[2]=(v/10)+'0';
  v-=((v/10)*10);

  buffer[3]=v+'0';
  buffer[4]='\0';

  gl.setForeground(ColourNames::WHITE);
  gl << p << buffer << "V";
}

