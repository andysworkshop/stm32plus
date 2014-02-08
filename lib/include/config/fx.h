/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This config file gets you access to the 'easing functions' that can be used to provide smooth animation
 * acceleration and deceleration algorithms that you see all over the place these days in everything from
 * PC GUIs to smartphones. The 'pframe' example uses easing functions to scroll in the next image.
 */

// fx depends on timing, math

#include "config/timing.h"
#include <math.h>

// sometimes absent from C++0x depending on source compatibility level

#if !defined(M_PI)
#define M_PI    3.14159265358979323846
#endif

#if !defined(M_PI_2)
#define M_PI_2    1.57079632679489661923
#endif

// includes for the feature

#include "fx/easing/EasingBase.h"
#include "fx/easing/BackEase.h"
#include "fx/easing/BounceEase.h"
#include "fx/easing/CircularEase.h"
#include "fx/easing/CubicEase.h"
#include "fx/easing/ElasticEase.h"
#include "fx/easing/ExponentialEase.h"
#include "fx/easing/LinearEase.h"
#include "fx/easing/QuadraticEase.h"
#include "fx/easing/QuarticEase.h"
#include "fx/easing/QuinticEase.h"
#include "fx/easing/SineEase.h"
