/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

TimeDisplay::TimeDisplay() {
  _time=888;
}


/*
 * Draw the time
 */

void TimeDisplay::drawTime(LcdPanel& gl,const Point& p) {

  char buffer[10];

  StringUtil::modp_uitoa10(_time,buffer);

  gl.setForeground(ColourNames::WHITE);
  gl << p << buffer << "ns";
}

