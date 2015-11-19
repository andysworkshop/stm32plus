/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#define SEMIHOSTING


#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/string.h"
#include "config/debug.h"
#include "config/can.h"




using namespace stm32plus;


/**
 * This example initialize the CAN peripheral at 400 kBits/s with 87.5% sampling point.
 * To get incoming messages needed to bypass the CAN filtering.
 * With the bypassing we get all messages to FIFO 0, so we need to enable the FMP0 interrupt.
 * After init it sends CAN frames in an infinite loop. With loopback mode we need to get all the sent messages
 *
 *
 *
 *
 * Compatible MCU:
 *   STM32F1
 *
 * Tested on devices:
 *   STM32F103C8T6
 *
 */


class CanSend {

  private:
    volatile bool _ready;
    uint8_t data[8];
    uint8_t i=0;

  public:

    void run() {

	/**
 	 *
	 *
	 */
     // Can1<Can1InterruptFeature,CanFilterBypassFeature> can( {400000,875});  // Sending, Receiving over CAN
    	Can1<Can1InterruptFeature,CanLoopbackModeFeature,CanFilterBypassFeature> can( {400000,875});// Sending, Loopback receiving over CAN


      can.CanInterruptEventSender.insertSubscriber(CanInterruptEventSourceSlot::bind(this,&CanSend::onCANInterrupt));
      can.enableInterrupts(CAN_IT_FMP0);


      /*
       * Go into an infinite loop sending
       */

      for(;;) {

        data[0]=i++;
        can.send(0x100,CAN_Id_Extended,CAN_RTR_Data,1,data);
        shost << "Message sent\r\n";
        MillisecondTimer::delay(1000);

      }
    }


    void onCANInterrupt(CanEventType cet) {

    	CanRxMsg RxMessage;
    	shost << "CAN interrupt arrived, ";


    	if(cet == CanEventType::EVENT_FIFO0_MESSAGE_PENDING)
    	{
    		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

    		shost << "data[0]: " <<RxMessage.Data[0] << "\r\n";


    	}
}

};




int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  // we need some delay
  MillisecondTimer::initialise();

  CanSend CS;
  CS.run();

  // not reached
  return 0;

}





