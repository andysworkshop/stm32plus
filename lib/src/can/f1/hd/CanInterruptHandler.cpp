/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F1_HD)

#include "config/can.h"

using namespace stm32plus;

// static initialisers for the hack that forces the IRQ handlers to be linked

Can1InterruptFeature::FPTR Can1InterruptFeature::_forceLinkage = nullptr;
CanEventSource *Can1InterruptFeature::_canInstance = nullptr;

extern "C"
{

#if defined(USE_CAN1_INTERRUPT)

void __attribute__ ((interrupt("IRQ"))) USB_HP_CAN1_TX_IRQHandler(void)
{

    if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_TRANSMIT_MAILBOX_EMPTY);
        CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
        __DSB(); // prevent erroneous recall of this handler due to delayed memory write
    }
}

void __attribute__ ((interrupt("IRQ"))) USB_LP_CAN1_RX0_IRQHandler(void)
{

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_MESSAGE_PENDING);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_FF0) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_FULL);
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_FOV0) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_OVR);
        CAN_ClearITPendingBit(CAN1, CAN_IT_FOV0);
    }
    __DSB(); // prevent erroneous recall of this handler due to delayed memory write
}

void __attribute__ ((interrupt("IRQ"))) CAN1_RX1_IRQHandler(void)
{

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP1) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_MESSAGE_PENDING);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_FF1) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_FULL);
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF1);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_FOV1) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_OVR);
        CAN_ClearITPendingBit(CAN1, CAN_IT_FOV1);
    }
    __DSB(); // prevent erroneous recall of this handler due to delayed memory write
}

void __attribute__ ((interrupt("IRQ"))) CAN1_SCE_IRQHandler(void)
{

    if (CAN_GetITStatus(CAN1, CAN_IT_EWG) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_WARNING);
        CAN_ClearITPendingBit(CAN1, CAN_IT_EWG);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_EPV) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_PASSIVE);
        CAN_ClearITPendingBit(CAN1, CAN_IT_EPV);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_BOF) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_BUS_OFF);
        CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_LEC) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_LAST_ERROR_CODE);
        CAN_ClearITPendingBit(CAN1, CAN_IT_LEC);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_WKU) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_WAKEUP);
        CAN_ClearITPendingBit(CAN1, CAN_IT_WKU);
    }
    else if (CAN_GetITStatus(CAN1, CAN_IT_SLK) != RESET)
    {
        Can1InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_SLEEP);
        CAN_ClearITPendingBit(CAN1, CAN_IT_SLK);
    }

    __DSB(); // prevent erroneous recall of this handler due to delayed memory write
}

#endif
}

#endif
