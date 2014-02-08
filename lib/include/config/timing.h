/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Timing, as opposed to timer, gets you access to the milli and microsecond timing and delay functionality
 * as well as the time provider implementations that use the RTC. Time providers are used by the file system
 * drivers to provide a timestamp when you create or modify a file or directory.
 */

// timing depends on timer, rtc

#include "config/timer.h"
#include "config/rtc.h"


// includes for the features that build on the timer

#include "timing/TimeProvider.h"
#include "timing/RtcTimeProvider.h"
#include "timing/NullTimeProvider.h"
#include "timing/MicrosecondDelay.h"
#include "timing/MillisecondTimer.h"
