/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file to get access to the USB peripheral.
 */

// ST driver includes

#include "usblib/device/core/inc/usbd_def.h"


// usb depends on ...

#include "config/event.h"


#if defined(STM32PLUS_F4)

#include "usb/UsbInternalFsPhy.h"
#include "usb/UsbCore.h"

#include "usb/device/events/UsbDeviceEventDescriptor.h"
#include "usb/device/UsbDeviceEventSource.h"
#include "usb/device/UsbDevice.h"
#include "usb/device/UsbHidDevice.h"

#endif

