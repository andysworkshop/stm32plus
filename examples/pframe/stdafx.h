/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "config/stm32plus.h"
#include "config/display/tft.h"
#include "config/display/touch.h"
#include "config/fx.h"
#include "config/sdcard.h"
#include "config/filesystem.h"

using namespace stm32plus;
using namespace stm32plus::display;
using namespace stm32plus::fx;

#include "LcdManager.h"
#include "Initialiser.h"
#include "FileSystemManager.h"
#include "BitmapManager.h"
#include "TouchManager.h"
#include "ImageTransitionAnimator.h"
#include "ImageManager.h"
#include "ThumbManager.h"
#include "PictureFrame.h"
