/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the NVIC peripheral functionality. The initialise function is used
 * to start up the Nvic with the priority grouping of your choice. The default is to start up in
 * NVIC_PriorityGroup_4 which provides 4 bits for interrupt priority and none for sub-priority. You can
 * change this to whatever you need.
 */

// generic includes for the peripheral

#include "nvic/NvicPeripheral.h"
