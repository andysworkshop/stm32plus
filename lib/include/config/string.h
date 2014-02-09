/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * Include this config file to get access to the string utilities. Numeric-to-ASCII and string tokenisation
 * utilities are the sort of thing you will find here.
 */

// string depends on scoped array, STL string, vector

#include "memory/scoped_array.h"
#include <iterator>
#include <vector>
#include <string>

// includes for the features

#include "string/StringUtil.h"
#include "string/TokenisedString.h"
#include "string/StdStringUtil.h"
#include "string/Ascii.h"
