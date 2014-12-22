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

  // MSC types

  #include "usb/device/msc/MscBotState.h"
  #include "usb/device/msc/MscBotStatus.h"
  #include "usb/device/msc/MscBotClassRequest.h"

  #include "usb/device/msc/MscScsiSense.h"
  #include "usb/device/msc/MscScsiCommand.h"
  #include "usb/device/msc/MscBotCswStatus.h"
  #include "usb/device/msc/MscBotCommandBlockWrapper.h"
  #include "usb/device/msc/MscBotCommandStatusWrapper.h"

  // MSC events

  #include "usb/device/msc/events/MscBotGetMaxLunEvent.h"
  #include "usb/device/msc/events/MscBotResetEvent.h"
  #include "usb/device/msc/events/MscBotIsReadyEvent.h"
  #include "usb/device/msc/events/MscBotIsWriteProtectedEvent.h"
  #include "usb/device/msc/events/MscBotGetInquiryPageEvent.h"
  #include "usb/device/msc/events/MscBotGetCapacityEvent.h"
  #include "usb/device/msc/events/MscBotReadEvent.h"
  #include "usb/device/msc/events/MscBotWriteEvent.h"

  // MSC device includes

  #include "usb/device/msc/MscScsi.h"
  #include "usb/device/msc/MscDevice.h"
  #include "usb/device/msc/BotMscDevice.h"

#endif

