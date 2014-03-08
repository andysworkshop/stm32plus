/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class DeltaTControl : public NonSelectableControl,
                      public TimeDisplay,
                      public DeltaDisplay {

  public:
    enum {
      CONTROL_X     = TControl::CONTROL_X+TControl::CONTROL_WIDTH+4,
      CONTROL_WIDTH = 41
    };

  public:
    virtual ~DeltaTControl() {}

    virtual void draw(Display& display) override;
};
