/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file gets you access to the STM32 ADC peripheral.
 */

// adc depends on rcc, dma

#include "config/rcc.h"
#include "config/dma.h"

// peripheral includes

#include "adc/AdcEventSource.h"
#include "adc/Adc.h"
#include "adc/AdcPeripheral.h"
#include "adc/Adc1.h"

// feature includes

#include "adc/features/AdcChannelGpioInitialiser.h"
#include "adc/features/AdcFeatureBase.h"
#include "adc/features/AdcRegularChannelFeature.h"
#include "adc/features/AdcClockPrescalerFeature.h"
#include "adc/features/AdcResolutionFeature.h"
#include "adc/features/AdcContinuousModeFeature.h"
#include "adc/features/AdcScanModeFeature.h"

// device specific feature includes

#if defined(STM32PLUS_F4)

#define USE_ADC_INTERRUPT

#include "adc/features/f4/AdcInterruptFeature.h"

#endif
