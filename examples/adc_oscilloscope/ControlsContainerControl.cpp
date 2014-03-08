/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

void ControlsContainerControl::draw(Display& display) {

  Rectangle rc;
  LcdPanel& gl(display.getGl());

  rc.X=0;
  rc.Y=LcdPanel::SHORT_SIDE-CONTROLS_HEIGHT;
  rc.Width=LcdPanel::LONG_SIDE;
  rc.Height=CONTROLS_HEIGHT;

  gl.setForeground(0x015c01);
  gl.fillRectangle(rc);
}
