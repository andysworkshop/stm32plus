/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class Display;

class Controls {

  public:
    enum {
      CONTROLS_CONTAINER = 0,
      CHANNEL_DIVIDER    = 1,
      OPERATING_MODE     = 2,
      TIME               = 3,
      DELTA_TIME         = 4,
      VOLTAGE            = 5,
      DELTA_VOLTAGE      = 6,

      NUM_CONTROLS       = 7,
    };

  protected:
    Control **_controls;
    uint8_t _selectedIndex;

  public:
    Controls();

    void drawAll(Display& display);

    OperatingModeType getOperatingMode() const;
};
