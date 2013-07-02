/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this file to get access to the raw SDIO sdcard functionality. Support is provided for access
 * by interrupts and/or DMA. SdioDmaSdCard is provided as a utility class that pulls everything together
 * to give access to the peripheral.
 */

// sdcard depends on rcc, device, gpio, nvic, dma, timing

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/device.h"
#include "config/nvic.h"
#include "config/dma.h"
#include "config/timing.h"

// use interrupts

#define USE_SDIO_INTERRUPT

// includes for the peripheral

#include "sdcard/SdCardBase.h"

// feature includes

#include "sdcard/features/NullSdCardFeature.h"
#include "sdcard/features/SdCardFeatureBase.h"
#include "sdcard/features/SdCardIoFeature.h"
#include "sdcard/features/SdCardSdioFeature.h"
#include "sdcard/features/SdioInterruptFeature.h"

// the template class

#include "sdcard/SdCard.h"

// utility classes

#include "sdcard/SdCardDetector.h"
#include "sdcard/SdioDmaSdCard.h"
