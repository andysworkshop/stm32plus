/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Manager class for the LCD display
 */

class LcdManager {

  public:

    // we'll use the ILI9325

    typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
    typedef ILI9325_Portrait_262K<LcdAccessMode> LcdAccess;
    typedef ILI9325_Terminal_Portrait<LcdAccess> TerminalAccess;

  protected:

    // members

    LcdAccessMode *_accessMode;
    LcdAccess *_lcd;
    TerminalAccess *_terminal;
    DefaultBacklight *_backlight;
    Font *_font;

  public:
    bool initialise();

    LcdAccess& getLcd() const {
      return *_lcd;
    }

    Font& getFont() const {
      return *_font;
    }

    TerminalAccess& getTerminal() const {
      return *_terminal;
    }
};
