/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class DeltaVControl : public NonSelectableControl,
                      public VoltageDisplay,
                      public DeltaDisplay {

  public:
    enum {
      CONTROL_X     = VControl::CONTROL_X+VControl::CONTROL_WIDTH+4,
      CONTROL_WIDTH = 41
    };

  public:
    virtual ~DeltaVControl() {}

    virtual void draw(Display& display) override;
};
