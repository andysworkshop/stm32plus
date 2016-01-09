/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if defined(STM32PLUS_F0_42)

  #define USE_USB_INTERRUPT

  // USB depends on gpio, event, nvic, bit hacks

  #include "config/gpio.h"
  #include "config/event.h"
  #include "config/nvic.h"
  #include "util/BitHacks.h"
  #include "util/StdExt.h"

  // core includes

  #include "usb/f0/UsbInterruptHandler.h"
  #include "usb/f0/UsbSelfPoweredFeature.h"
  #include "usb/f0/UsbDefs.h"
  #include "usb/f0/UsbStatusType.h"
  #include "usb/f0/UsbBufferDescriptionTableEntry.h"
  #include "usb/f0/UsbSetupHeader.h"
  #include "usb/f0/UsbInEndpointData.h"
  #include "usb/f0/UsbConfigurationFlags.h"

  // device type includes

  #include "usb/f0/hid/UsbCustomHid.h"

#elif defined(STM32PLUS_F4)

  /*
   * Don't include this file directly in application code, instead you should include the correct
   * device class (config/usb/f4/device/cdc.h, config/usb/f4/device/hid.h etc...)
   */

  #define USE_OTG_FS_INTERRUPT

  // ST driver includes

  #include "usblib/hal/inc/stm32f4xx_ll_usb.h"
  #include "usblib/hal/inc/stm32f4xx_hal_pcd.h"
  #include "usblib/hal/inc/stm32f4xx_hal_pcd_ex.h"

  #include "config/event.h"

  #include "usb/f4/UsbEventDescriptor.h"
  #include "usb/f4/UsbErrorEvent.h"
  #include "usb/f4/UsbEventSource.h"

  #include "usblib/device/core/inc/usbd_def.h"
  #include "usblib/device/core/inc/usbd_core.h"

  // usb depends on ...

  #include "config/timing.h"
  #include "config/smartptr.h"
  #include "config/rcc.h"
  #include "config/exti.h"
  #include "util/Meta.h"
  #include "util/StdExt.h"

  #include "usb/f4/device/HalUsbdStatus.h"

  #include "usb/f4/phy/features/PhyFeatureBase.h"
  #include "usb/f4/phy/events/OtgFsInterruptEvent.h"
  #include "usb/f4/phy/features/FsLowPowerFeature.h"
  #include "usb/f4/phy/features/InternalFsPhyInterruptFeature.h"
  #include "usb/f4/phy/InternalFsPhy.h"
  #include "usb/f4/core/Core.h"

  // device-level includes

  #include "usb/f4/device/DescriptorHeader.h"
  #include "usb/f4/device/ConfigurationDescriptor.h"
  #include "usb/f4/device/InterfaceDescriptor.h"
  #include "usb/f4/device/InterfaceAssociationDescriptor.h"
  #include "usb/f4/device/EndpointDescriptor.h"
  #include "usb/f4/device/DeviceDescriptor.h"
  #include "usb/f4/device/DeviceQualifierDescriptor.h"
  #include "usb/f4/device/LanguageDescriptor.h"
  #include "usb/f4/device/EndpointType.h"

  // device event includes

  #include "usb/f4/device/events/DeviceSdkNotifyEvent.h"
  #include "usb/f4/device/events/DeviceSdkEndpointEvent.h"
  #include "usb/f4/device/events/DeviceSdkOpenEndpointEvent.h"
  #include "usb/f4/device/events/DeviceSdkIsStalledEndpointEvent.h"
  #include "usb/f4/device/events/DeviceSdkDataEndpointEvent.h"
  #include "usb/f4/device/events/DeviceSdkGetLastTransferredSizeEndpointEvent.h"
  #include "usb/f4/device/events/DeviceGetDeviceDescriptorEvent.h"
  #include "usb/f4/device/events/DeviceGetLanguageDescriptorEvent.h"
  #include "usb/f4/device/events/DeviceGetDisplayStringDescriptorEvent.h"
  #include "usb/f4/device/events/DeviceSdkResetInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkSuspendInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkResumeInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkSetupStageInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkConnectInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkDisconnectInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkDataOutStageInterruptEvent.h"
  #include "usb/f4/device/events/DeviceSdkDataInStageInterruptEvent.h"

  #include "usb/f4/device/events/class/DeviceClassSdkInitEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkDeInitEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkDataInEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkDataOutEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkEp0ReadyEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkEp0TxSentEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkGetConfigurationDescriptorEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkGetDeviceQualifierDescriptorEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkSetupEvent.h"
  #include "usb/f4/device/events/class/DeviceClassSdkSofEvent.h"

  // device features

  #include "usb/f4/device/Device.h"
  #include "usb/f4/device/DeviceClass.h"

  #include "usb/f4/device/features/DeviceFeatureBase.h"
  #include "usb/f4/device/features/endpoints/ControlEndpointFeature.h"
  #include "usb/f4/device/features/endpoints/InEndpointFeatureBase.h"
  #include "usb/f4/device/features/endpoints/OutEndpointFeatureBase.h"
  #include "usb/f4/device/features/endpoints/InterruptInEndpointFeature.h"
  #include "usb/f4/device/features/endpoints/InterruptOutEndpointFeature.h"
  #include "usb/f4/device/features/endpoints/BulkInEndpointFeature.h"
  #include "usb/f4/device/features/endpoints/BulkOutEndpointFeature.h"
  #include "usb/f4/device/features/StringFeatureBase.h"
  #include "usb/f4/device/features/ManufacturerTextFeature.h"
  #include "usb/f4/device/features/ProductTextFeature.h"
  #include "usb/f4/device/features/SerialNumberTextFeature.h"
  #include "usb/f4/device/features/ConfigurationTextFeature.h"
  #include "usb/f4/device/features/InterfaceTextFeature.h"

#endif

