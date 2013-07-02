/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Base class for classes that need to perform startup tasks
 */

class Initialiser {

  protected:
    LcdManager& _lcdManager;
    LcdManager::TerminalAccess& _term;

  public:
    virtual ~Initialiser() {}

  protected:
    bool error(const char *message);

    Initialiser(LcdManager& lcdManager);

    virtual bool initialise()=0;
};
