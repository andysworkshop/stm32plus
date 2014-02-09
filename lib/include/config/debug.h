/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file provides access to generic utilities that can be used to
 * assist debugging
 */

// debug depends on usart,stream,malloc.h

#include <malloc.h>
#include "config/usart.h"
#include "config/stream.h"

// include the features

#include "include/debug/heapMonitor.h"
#include "include/debug/usartHeapMonitor.h"
