/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#if defined(STM32PLUS_F4)

  // device base include

  #include "config/usb/device/device.h"

  // MSC descriptors

  #include "usb/device/msc/MscProtocol.h"
  #include "usb/device/msc/MscSubClass.h"

  // MSC device includes

  #include "usb/device/msc/MscDevice.h"
  #include "usb/device/msc/BotMscDevice.h"

#endif

