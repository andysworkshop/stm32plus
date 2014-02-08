/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This file provides access to a random number generator. On the F4 the hardware RNG is used. On
 * the F1 we fall back to an implementation of WELL512a which has good randomness properties but
 * not good enough for secure crypto.
 */

// RNG depends on rcc, nvic, event

#include "config/rcc.h"
#include "config/nvic.h"
#include "config/event.h"

// device specific includes

#if defined(STM32PLUS_F4)

#define USE_RNG_INTERRUPT

#include "rng/f4/Rng.h"
#include "rng/f4/RngEventSource.h"
#include "rng/f4/features/RngInterruptFeature.h"

#elif defined(STM32PLUS_F1)

#include "rng/f1/Well512.h"

#else
#error "Unsupported MCU"
#endif
