/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

Initialiser::Initialiser(LcdManager& lcdManager)
  : _lcdManager(lcdManager),
    _term(_lcdManager.getTerminal()) {
}


/*
 * Return an error
 */

bool Initialiser::error(const char *message) {

  _term << message << '\n';
  return false;
}
