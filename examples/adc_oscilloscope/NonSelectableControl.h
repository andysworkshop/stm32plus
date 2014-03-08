/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class NonSelectableControl : public Control {

  protected:
    void clearRectangle(LcdPanel& gl,const Rectangle& rc);

  // overrides from control

  virtual bool isSelectable() const override {
    return false;
  }
};
