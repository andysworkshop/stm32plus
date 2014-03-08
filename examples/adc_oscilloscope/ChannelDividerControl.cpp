/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

void ChannelDividerControl::draw(Display& display) {

  Point p[2];
  LcdPanel& gl(display.getGl());

  // draw the divider between channels

  p[0].X=0;
  p[0].Y=p[1].Y=(LcdPanel::SHORT_SIDE-ControlsContainerControl::CONTROLS_HEIGHT)/2;
  p[1].X=LcdPanel::LONG_SIDE-1;

  gl.setForeground(0x888888);
  gl.drawLine(p[0],p[1]);
}
