/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class OperatingModeControl : public SelectableControl {

  public:
    enum {
      CONTROL_X     = 4,
      CONTROL_WIDTH = 15
    };

  protected:
    OperatingModeType _mode;

  public:
    OperatingModeControl();
    virtual ~OperatingModeControl() {}

    virtual void draw(Display& display);

    OperatingModeType getOperatingMode() const;
};


/*
 * Trivial constructor
 */

inline OperatingModeControl::OperatingModeControl() {
  _mode=OperatingModeType::CONTINUOUS;
}


/*
 * Get the operating mode
 */

inline OperatingModeType OperatingModeControl::getOperatingMode() const {
  return _mode;
}
