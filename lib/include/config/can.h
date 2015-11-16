/*
 * can.h
 *
 *  Created on: 2015. nov. 10.
 *      Author: elektroman
 */

#pragma once

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/event.h"
#include "util/Meta.h"





// device-specific pin initialiser

#include "can/CanPiniInitialiser.h"

#include "can/CanEventSource.h"

// generic peripheral includes

#include "can/Can.h"
#include "can/CanPeripheral.h"
#include "can/CanFeatureBase.h"
#include "can/Can1.h"
#define USE_CAN1_INTERRUPT


#include "can/features/CanFilterBypassFeature.h"
#include "can/features/CanSilentModeFeature.h"
#include "can/features/CanLoopbackFeature.h"

#include "can/features/CanInterruptFeature.h"



