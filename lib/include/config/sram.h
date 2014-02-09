/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this file to get access to the SRAM functionality provided by the FSMC peripheral. A driver is
 * provided for the ISSI IS61LV25616 device.
 */

// sram depends on fsmc, memory copy, device

#include "config/fsmc.h"
#include "config/device.h"
#include "memory/MEM_DataCopy.h"

// includes for the memory devices

#include "sram/GenericSram.h"
#include "sram/IS61LV25616.h"
