/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


class SelectableControl : public Control {

  protected:
    bool _selected;

  protected:
    void clearRectangle(LcdPanel& gl,const Rectangle& rc);

  public:
    SelectableControl();
    virtual ~SelectableControl() {}

    void setSelected(bool selected);

    // overrides from control

    virtual bool isSelectable() const override;
};


/*
 * Trivial constructor
 */

inline SelectableControl::SelectableControl() {
  _selected=false;
}


/*
 * Set the selected flag
 */

inline void SelectableControl::setSelected(bool selected) {
  _selected=selected;
}


/*
 * Return that this is selectable
 */

inline bool SelectableControl::isSelectable() const {
  return true;
}
