/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

void DeltaVControl::draw(Display& display) {

  LcdPanel& gl(display.getGl());

  // erase the background

  Rectangle rc(CONTROL_X,
               LcdPanel::SHORT_SIDE-ControlsContainerControl::CONTROLS_HEIGHT+1,
               CONTROL_WIDTH,
               ControlsContainerControl::CONTROLS_HEIGHT-2);

  clearRectangle(gl,rc);

  // draw the delta

  drawDelta(gl,Point(rc.X+2+2,rc.Y+4));

  // draw the voltage

  drawVoltage(gl,Point(rc.X+2+8,rc.Y+2));
}
