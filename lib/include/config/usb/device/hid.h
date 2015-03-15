/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#if defined(STM32PLUS_F4)

  // device base include

  #include "config/usb/device/device.h"

  // HID device includes

  #include "usb/device/hid/HidClassRequestType.h"
  #include "usb/device/hid/HidClassDescriptor.h"
  #include "usb/device/hid/HidSubClass.h"
  #include "usb/device/hid/HidProtocol.h"

  #include "usb/device/hid/events/HidKeyboardLedStateEvent.h"

  #include "usb/device/hid/HidDevice.h"
  #include "usb/device/hid/MouseReportDescriptor.h"
  #include "usb/device/hid/KeyboardReportDescriptor.h"
  #include "usb/device/hid/MouseHidDevice.h"
  #include "usb/device/hid/KeyboardHidDevice.h"

#endif

