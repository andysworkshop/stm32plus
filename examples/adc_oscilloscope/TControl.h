/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class TControl : public NonSelectableControl,
                 public TimeDisplay {

  public:
    enum {
      CONTROL_X     = OperatingModeControl::CONTROL_X+OperatingModeControl::CONTROL_WIDTH+4,
      CONTROL_WIDTH = 32
    };

  public:
    virtual ~TControl() {}

    virtual void draw(Display& display) override;
};
