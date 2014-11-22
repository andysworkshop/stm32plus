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

#define USE_OTG_FS_INTERRUPT

// ST driver includes

#include "usblib/hal/inc/stm32f4xx_ll_usb.h"
#include "usblib/hal/inc/stm32f4xx_hal_pcd.h"
#include "usblib/hal/inc/stm32f4xx_hal_pcd_ex.h"

#include "usblib/device/core/inc/usbd_def.h"
#include "usblib/device/core/inc/usbd_core.h"

// usb depends on ...

#include "config/event.h"
#include "config/timing.h"
#include "config/smartptr.h"
#include "config/rcc.h"
#include "config/exti.h"
#include "util/Meta.h"
#include "util/StdExt.h"
#include "util/MinMax.h"

#if defined(STM32PLUS_F4)

  #include "usb/device/HalUsbdStatus.h"

  #include "usb/UsbEventDescriptor.h"
  #include "usb/UsbErrorEvent.h"
  #include "usb/UsbEventSource.h"

  #include "usb/phy/features/PhyFeatureBase.h"
  #include "usb/phy/events/OtgFsInterruptEvent.h"
  #include "usb/phy/features/FsLowPowerFeature.h"
  #include "usb/phy/features/InternalFsPhyInterruptFeature.h"
  #include "usb/phy/InternalFsPhy.h"
  #include "usb/core/Core.h"

  #include "usb/device/DescriptorHeader.h"
  #include "usb/device/ConfigurationDescriptor.h"
  #include "usb/device/InterfaceDescriptor.h"
  #include "usb/device/EndpointDescriptor.h"
  #include "usb/device/DeviceDescriptor.h"
  #include "usb/device/DeviceQualifierDescriptor.h"
  #include "usb/device/LanguageDescriptor.h"
  #include "usb/device/EndpointType.h"

  #include "usb/device/events/DeviceSdkNotifyEvent.h"
  #include "usb/device/events/DeviceSdkEndpointEvent.h"
  #include "usb/device/events/DeviceSdkOpenEndpointEvent.h"
  #include "usb/device/events/DeviceSdkIsStalledEndpointEvent.h"
  #include "usb/device/events/DeviceSdkDataEndpointEvent.h"
  #include "usb/device/events/DeviceSdkGetLastTransferredSizeEndpointEvent.h"
  #include "usb/device/events/DeviceGetDeviceDescriptorEvent.h"
  #include "usb/device/events/DeviceGetLanguageDescriptorEvent.h"
  #include "usb/device/events/DeviceGetDisplayStringDescriptorEvent.h"
  #include "usb/device/events/DeviceSdkResetInterruptEvent.h"
  #include "usb/device/events/DeviceSdkSuspendInterruptEvent.h"
  #include "usb/device/events/DeviceSdkResumeInterruptEvent.h"
  #include "usb/device/events/DeviceSdkSetupStageInterruptEvent.h"
  #include "usb/device/events/DeviceSdkConnectInterruptEvent.h"
  #include "usb/device/events/DeviceSdkDisconnectInterruptEvent.h"
  #include "usb/device/events/DeviceSdkDataOutStageInterruptEvent.h"
  #include "usb/device/events/DeviceSdkDataInStageInterruptEvent.h"

  #include "usb/device/Device.h"
  #include "usb/device/DeviceClass.h"

  #include "usb/device/features/DeviceFeatureBase.h"
  #include "usb/device/features/endpoints/ControlEndpointFeature.h"
  #include "usb/device/features/endpoints/InEndpointFeatureBase.h"
  #include "usb/device/features/endpoints/OutEndpointFeatureBase.h"
  #include "usb/device/features/endpoints/InterruptInEndpointFeature.h"
  #include "usb/device/features/endpoints/InterruptOutEndpointFeature.h"
  #include "usb/device/features/StringFeatureBase.h"
  #include "usb/device/features/ManufacturerTextFeature.h"
  #include "usb/device/features/ProductTextFeature.h"
  #include "usb/device/features/SerialNumberTextFeature.h"
  #include "usb/device/features/ConfigurationTextFeature.h"
  #include "usb/device/features/InterfaceTextFeature.h"

  #include "usb/device/hid/HidClassRequestType.h"
  #include "usb/device/hid/HidClassDescriptor.h"
  #include "usb/device/hid/HidSubClass.h"
  #include "usb/device/hid/HidProtocol.h"

  #include "usb/device/hid/events/HidSdkInitEvent.h"
  #include "usb/device/hid/events/HidSdkDeInitEvent.h"
  #include "usb/device/hid/events/HidSdkDataInEvent.h"
  #include "usb/device/hid/events/HidSdkDataOutEvent.h"
  #include "usb/device/hid/events/HidSdkEp0ReadyEvent.h"
  #include "usb/device/hid/events/HidSdkGetConfigurationDescriptorEvent.h"
  #include "usb/device/hid/events/HidSdkGetDeviceQualifierDescriptorEvent.h"
  #include "usb/device/hid/events/HidSdkSetupEvent.h"

  #include "usb/device/hid/HidDevice.h"
  #include "usb/device/hid/MouseReportDescriptor.h"
  #include "usb/device/hid/KeyboardReportDescriptor.h"
  #include "usb/device/hid/MouseHidDevice.h"
  #include "usb/device/hid/KeyboardHidDevice.h"

#endif

