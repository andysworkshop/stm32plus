/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the HTTP application layer feature.
 */

#if defined(STM32PLUS_F4) || defined(STM32PLUS_F1_CL_E)


// net_http depends on net, stream

#include "config/stream.h"
#include "config/net.h"

// includes for the protocol

#include "net/application/http/HttpVersion.h"
#include "net/application/http/HttpMethod.h"
#include "net/application/http/HttpServerConnection.h"
#include "net/application/http/HttpClient.h"


#endif
