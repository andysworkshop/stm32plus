/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file enables access to the HASH peripheral in the F4 for performing cryptographic
 * hash functions. The functionality is emulated in software for devices that don't have the
 * peripheral.
 */

// hash depends on timing

#include "config/timing.h"

// device-specific peripheral includes

#if defined(STM32PLUS_F4) && defined(STM32PLUS_F4_HARDWARE_CRYPTO)
  #include "hash/f4/SHA1.h"
#else
  #include "hash/software/SHA1.h"
#endif

// generic peripheral includes

#include "hash/HashPeripheral.h"

