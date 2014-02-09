/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


/**
 * @file
 * This config file gives access to the concurrency-related utility classes such as Mutex, Critical Section
 * and other classes. These are useful when sharing data between IRQ and normal code paths.
 */

#include "config/timing.h"

#include "concurrent/CriticalSection.h"
#include "concurrent/atomic.h"
#include "concurrent/IrqSuspend.h"

// mutex only on cortex M3 and above due to the need for strex/ldrex* instructions

#if !defined(STM32PLUS_F0)
  #include "concurrent/Mutex.h"
#endif
