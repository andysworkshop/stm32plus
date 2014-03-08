/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Base class for controls
 */

class Display;

struct Control {

  virtual ~Control() {}

  virtual bool isSelectable() const=0;
  virtual void draw(Display& display)=0;
};
