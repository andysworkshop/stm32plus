/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file gets you access to the EEPROM functionality.
 */

// eeprom depends on i2c, input stream, output stream, minmax

#include "config/i2c.h"
#include "config/stream.h"

// general includes

#include "eeprom/SerialEeprom.h"

// device includes

#include "eeprom/AT24Cxx.h"
#include "eeprom/BR24G32.h"
