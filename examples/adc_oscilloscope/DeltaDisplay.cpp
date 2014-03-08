/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Draw the delta symbol
 */

void DeltaDisplay::drawDelta(LcdPanel& gl,const Point& pTop) {

  Point p[2];
  uint8_t i;

  gl.setForeground(ColourNames::WHITE);

  p[0]=p[1]=pTop;

  for(i=0;i<3;i++) {
    gl.drawLine(p[0],p[1]);

    p[0].X--;
    p[1].X++;
    p[0].Y++;
    p[1].Y++;
  }
}
