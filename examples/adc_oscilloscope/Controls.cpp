/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


Controls::Controls() {

  _controls=new Control *[NUM_CONTROLS];

  _controls[CONTROLS_CONTAINER]=new ControlsContainerControl;
  _controls[CHANNEL_DIVIDER]=new ChannelDividerControl;
  _controls[OPERATING_MODE]=new OperatingModeControl;
  _controls[TIME]=new TControl;
  _controls[DELTA_TIME]=new DeltaTControl;
  _controls[VOLTAGE]=new VControl;
  _controls[DELTA_VOLTAGE]=new DeltaVControl;

  _selectedIndex=OPERATING_MODE;
  static_cast<OperatingModeControl *>(_controls[OPERATING_MODE])->setSelected(true);
}


void Controls::drawAll(Display& display) {

  uint8_t i;

  for(i=0;i<NUM_CONTROLS;i++)
    _controls[i]->draw(display);
}


/*
 * Get the operating mode
 */

OperatingModeType Controls::getOperatingMode() const {
  return static_cast<OperatingModeControl *>(_controls[OPERATING_MODE])->getOperatingMode();
}
