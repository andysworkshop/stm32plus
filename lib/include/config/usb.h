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

#include "usblib/hal/inc/stm32f4xx_ll_usb.h"
#include "usblib/hal/inc/stm32f4xx_hal_pcd.h"
#include "usblib/hal/inc/stm32f4xx_hal_pcd_ex.h"

#include "usblib/device/core/inc/usbd_def.h"
#include "usblib/device/core/inc/usbd_core.h"

#include "usblib/device/class/hid/inc/usbd_hid.h"

// usb depends on ...

#include "config/event.h"
#include "config/timing.h"
#include "config/smartptr.h"
#include "config/rcc.h"
#include "config/exti.h"

#if defined(STM32PLUS_F4)

  #include "usb/phy/features/FsLowPowerFeature.h"
  #include "usb/phy/InternalFsPhy.h"
  #include "usb/core/Core.h"

  #include "usb/device/DeviceDescriptor.h"
  #include "usb/device/LanguageDescriptor.h"
  #include "usb/device/EndpointType.h"

  #include "usb/device/events/DeviceEventDescriptor.h"
  #include "usb/device/events/DeviceSdkNotifyEvent.h"
  #include "usb/device/events/DeviceSdkEndpointEvent.h"
  #include "usb/device/events/DeviceSdkOpenEndpointEvent.h"
  #include "usb/device/events/DeviceSdkIsStalledEndpointEvent.h"
  #include "usb/device/events/DeviceSdkDataEndpointEvent.h"
  #include "usb/device/events/DeviceSdkGetLastTransferredSizeEndpointEvent.h"
  #include "usb/device/events/DeviceGetDeviceDescriptorEvent.h"
  #include "usb/device/events/DeviceGetLanguageDescriptorEvent.h"
  #include "usb/device/events/DeviceGetDisplayStringDescriptorEvent.h"

  #include "usb/device/DeviceEventSource.h"
  #include "usb/device/DeviceBase.h"
  #include "usb/device/Device.h"

  #include "usb/device/features/DeviceFeatureBase.h"
  #include "usb/device/features/endpoints/ControlEndpointFeature.h"
  #include "usb/device/features/endpoints/InEndpointFeatureBase.h"
  #include "usb/device/features/endpoints/InterruptInEndpointFeature.h"
  #include "usb/device/features/StringFeatureBase.h"
  #include "usb/device/features/ManufacturerTextFeature.h"
  #include "usb/device/features/ProductTextFeature.h"
  #include "usb/device/features/SerialNumberTextFeature.h"
  #include "usb/device/features/ConfigurationTextFeature.h"
  #include "usb/device/features/InterfaceTextFeature.h"

  #include "usb/device/HidDevice.h"

#endif

