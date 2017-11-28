/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file to get access to the C++ event system. The event system allows you to raise
 * type-safe events that listeners can subscribe to. This system avoids the use of the void* pointers
 * that the Observer/Observable system uses and allows event arguments to be tightly coupled to the
 * class that raises them.
 */

// event depends on stl slist
// some implementation contain slist in ext/slist
#include "iterator"
#ifdef EXT_SLIST
    #include "ext/slist"
#else
    #include "slist"
#endif

// include the event/slot signal classes

#include "event/slot.h"
#include "event/signal.h"

// macros for declaring the event signature and source class

#define DECLARE_EVENT_SIGNATURE(name,sig) typedef wink::slot<sig> name##EventSourceSlot; typedef wink::signal<name##EventSourceSlot> name##EventSourceType
#define DECLARE_EVENT_SOURCE(name) name##EventSourceType name##EventSender
