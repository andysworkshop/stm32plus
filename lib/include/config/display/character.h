/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file if you want to use any of the character based LCD functionality. Currently
 * this gets you access to the HD44780 driver and character terminal.
 */

// character display depends on gpio, timing, string, double precision

#include "config/gpio.h"
#include "config/timing.h"
#include "config/string.h"
#include "util/DoublePrecision.h"

// includes for the devices

#include "display/character/CharacterLcd.h"
#include "display/character/HD44780.h"

// includes for the features

#include "display/Point.h"
#include "display/Size.h"

#include "display/character/CharacterLcdTerminal.h"
