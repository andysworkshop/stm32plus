/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Clear a rectangle to the normal or selected state
 */

void SelectableControl::clearRectangle(LcdPanel& gl,const Rectangle& rc) {

  gl.setBackground(_selected ? 0x030baa : ColourNames::BLACK);
  gl.clearRectangle(rc);
}
