/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Main entry point
 */

int main() {

  PictureFrame pf;

  pf.run();

  for(;;);

  // not reached
  return 0;
}
