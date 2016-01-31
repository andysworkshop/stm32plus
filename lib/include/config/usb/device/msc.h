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

  #include "usb/f4/device/msc/MscProtocol.h"
  #include "usb/f4/device/msc/MscSubClass.h"

  // MSC types

  #include "usb/f4/device/msc/MscBotState.h"
  #include "usb/f4/device/msc/MscBotStatus.h"
  #include "usb/f4/device/msc/MscBotClassRequest.h"

  #include "usb/f4/device/msc/MscScsiSense.h"
  #include "usb/f4/device/msc/MscScsiCommand.h"
  #include "usb/f4/device/msc/MscBotCswStatus.h"
  #include "usb/f4/device/msc/MscBotCommandBlockWrapper.h"
  #include "usb/f4/device/msc/MscBotCommandStatusWrapper.h"

  // MSC events

  #include "usb/f4/device/msc/events/MscBotGetMaxLunEvent.h"
  #include "usb/f4/device/msc/events/MscBotResetEvent.h"
  #include "usb/f4/device/msc/events/MscBotIsReadyEvent.h"
  #include "usb/f4/device/msc/events/MscBotIsWriteProtectedEvent.h"
  #include "usb/f4/device/msc/events/MscBotGetInquiryPageEvent.h"
  #include "usb/f4/device/msc/events/MscBotGetCapacityEvent.h"
  #include "usb/f4/device/msc/events/MscBotReadEvent.h"
  #include "usb/f4/device/msc/events/MscBotWriteEvent.h"

  // MSC device includes

  #include "usb/f4/device/msc/MscScsi.h"
  #include "usb/f4/device/msc/MscDevice.h"
  #include "usb/f4/device/msc/BotMscDevice.h"

#endif

