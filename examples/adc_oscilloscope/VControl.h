/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class VControl : public NonSelectableControl,
                 public VoltageDisplay {

  public:
    enum {
      CONTROL_X     = DeltaTControl::CONTROL_X+DeltaTControl::CONTROL_WIDTH+4,
      CONTROL_WIDTH = 31
    };

  public:
    virtual ~VControl() {}

    virtual void draw(Display& display) override;
};
