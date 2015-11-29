/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#if defined(STM32PLUS_F4)

  // device base include

  #include "config/usb/device/device.h"

  // CDC device includes

  #include "usb/f4/device/cdc/CdcControlCommand.h"
  #include "usb/f4/device/cdc/CdcLineCoding.h"

  #include "usb/f4/device/cdc/events/CdcDataReceivedEvent.h"
  #include "usb/f4/device/cdc/events/CdcControlEvent.h"

  #include "usb/f4/device/cdc/CdcProtocol.h"
  #include "usb/f4/device/cdc/CdcSubClass.h"
  #include "usb/f4/device/cdc/CdcFunctionalDescriptor.h"
  #include "usb/f4/device/cdc/CdcHeaderFunctionalDescriptor.h"
  #include "usb/f4/device/cdc/CdcOneSlaveUnionFunctionalDescriptor.h"
  #include "usb/f4/device/cdc/CdcCallManagementFunctionalDescriptor.h"
  #include "usb/f4/device/cdc/CdcAbstractControlManagementFunctionalDescriptor.h"

  #include "usb/f4/device/cdc/CdcDevice.h"
  #include "usb/f4/device/cdc/ComPortCdcDevice.h"

#endif

