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

// adc depends on rcc, dma, meta-programming utils

#include "config/rcc.h"
#include "config/dma.h"
#include "util/Meta.h"

// peripheral includes

#include "adc/AdcEventSource.h"
#include "adc/Adc.h"
#include "adc/AdcPeripheral.h"
#include "adc/Adc1.h"
#include "adc/Adc2.h"
#include "adc/Adc3.h"

// feature includes

#include "adc/features/AdcChannelGpioInitialiser.h"
#include "adc/features/AdcFeatureBase.h"
#include "adc/features/AdcRegularChannelFeature.h"
#include "adc/features/AdcBatteryVoltageFeature.h"
#include "adc/features/AdcInternalReferenceVoltageFeature.h"
#include "adc/features/AdcTemperatureSensorFeature.h"
#include "adc/features/AdcClockPrescalerFeature.h"
#include "adc/features/AdcResolutionFeature.h"
#include "adc/features/AdcContinuousModeFeature.h"
#include "adc/features/AdcScanModeFeature.h"
#include "adc/features/AdcTriggerFeature.h"

// device specific feature includes

#if defined(STM32PLUS_F4)

#define USE_ADC_INTERRUPT

#include "adc/features/f4/AdcInterruptFeature.h"
#include "adc/features/f4/AdcInjectedChannelFeatureBase.h"
#include "adc/features/f4/AdcInjectedChannelFeature.h"

#endif
