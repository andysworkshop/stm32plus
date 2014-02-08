/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the built-in support for push-button logic. There is a plain
 * pushbutton class and one that supports auto-repeat.
 */

// buttons depend on gpio, event, MillisecondTimer

#include "config/gpio.h"
#include "config/event.h"
#include "timing/MillisecondTimer.h"

// include the buttons

#include "button/PushButton.h"
#include "button/AutoRepeatPushButton.h"
