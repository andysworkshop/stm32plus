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

  #include "usb/device/cdc/CdcControlCommand.h"
  #include "usb/device/cdc/CdcLineCoding.h"

  #include "usb/device/cdc/events/CdcDataReceivedEvent.h"
  #include "usb/device/cdc/events/CdcControlEvent.h"

  #include "usb/device/cdc/CdcProtocol.h"
  #include "usb/device/cdc/CdcSubClass.h"
  #include "usb/device/cdc/CdcFunctionalDescriptor.h"
  #include "usb/device/cdc/CdcHeaderFunctionalDescriptor.h"
  #include "usb/device/cdc/CdcOneSlaveUnionFunctionalDescriptor.h"
  #include "usb/device/cdc/CdcCallManagementFunctionalDescriptor.h"
  #include "usb/device/cdc/CdcAbstractControlManagementFunctionalDescriptor.h"

  #include "usb/device/cdc/CdcDevice.h"
  #include "usb/device/cdc/ComPortCdcDevice.h"

#endif

