/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This is an internal config file that is included as a dependency by any of the peripherals that expose
 * something through the Observer pattern. Generally this will be anything that raises interrupts.
 */

// includes for the feature

#include "observer/ObservableEvent.h"
#include "observer/Observable.h"
#include "observer/ObserverLink.h"
#include "observer/Observer.h"
#include "observer/ObserverLinkManager.h"
