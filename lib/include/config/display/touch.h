/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// touch depends on event, stream, exti, millisecond timer, spi

#include "config/event.h"
#include "config/stream.h"
#include "config/exti.h"
#include "config/spi.h"
#include "timing/MillisecondTimer.h"

// includes for the features

#include "display/Point.h"
#include "display/Size.h"

#include "display/touch/TouchScreenCalibration.h"
#include "display/touch/TouchScreenPostProcessor.h"
#include "display/touch/TouchScreen.h"
#include "display/touch/AveragingTouchScreenPostProcessor.h"
#include "display/touch/PassThroughTouchScreenCalibration.h"
#include "display/touch/PassThroughTouchScreenPostProcessor.h"
#include "display/touch/ThreePointTouchScreenCalibration.h"
#include "display/touch/TouchScreenCalibrator.h"
#include "display/touch/ThreePointTouchScreenCalibrator.h"

#include "display/touch/ADS7843AsyncTouchScreen.h"
