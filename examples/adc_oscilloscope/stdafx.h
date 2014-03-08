/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// library includes

#include "config/stm32plus.h"
#include "config/adc.h"
#include "config/timing.h"
#include "config/display/tft.h"

// we'll be lazy and import the whole library namespace

using namespace stm32plus;
using namespace stm32plus::display;

// local includes

#include "OperatingModeType.h"
#include "Control.h"
#include "Controls.h"
#include "Display.h"
#include "TimeDisplay.h"
#include "DeltaDisplay.h"
#include "VoltageDisplay.h"
#include "SelectableControl.h"
#include "NonSelectableControl.h"
#include "OperatingModeControl.h"
#include "ControlsContainerControl.h"
#include "ChannelDividerControl.h"
#include "TControl.h"
#include "DeltaTControl.h"
#include "VControl.h"
#include "DeltaVControl.h"
#include "AdcContinuous.h"
#include "AdcTriggered.h"
#include "Oscilloscope.h"
