/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once
#include "fwlib/f1/stdperiph/inc/stm32f10x_can.h"

/*
 * Forward declare the IRQ handler names
 */

extern "C" void USB_HP_CAN1_TX_IRQHandler(); /*!< USB Device High Priority or CAN1 TX Interrupts       */
extern "C" void USB_LP_CAN1_RX0_IRQHandler();/*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
extern "C" void CAN1_RX1_IRQHandler(); 		 /*!< CAN1 RX1 Interrupt                                   */
extern "C" void CAN1_SCE_IRQHandler();		 /*!< CAN1 SCE Interrupt                                   */


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each CAN peripheral
   * @tparam TCANNumber The number of the I2C peripheral (1..2)
   */


template<uint8_t TCANNumber>
class CanInterruptFeatureEnabler{
private:
	typedef void (*FPTR)();// this trick will force the linker to include the ISR
    static FPTR _forceLinkage;

  public:
    static void enable(uint8_t priority,uint8_t subPriority);
};


/**
  * I2C feature to handle interrupts
  * @tparam TI2CNumber The number of the I2C peripheral (1..5)
  */
template<uint8_t TCANNumber>
class CanInterruptFeature : public CanEventSource,
							public CanFeatureBase {
	protected:
	  uint16_t _interruptMask;
	  uint8_t _nvicPriority;
	  uint8_t _nvicSubPriority;

	public:
	  static CanEventSource *_canInstance;

	public:
	  CanInterruptFeature(Can& can);
	  ~CanInterruptFeature();

	  void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

	  void enableInterrupts(uint16_t interruptMask);
	  void disableInterrupts(uint16_t interruptMask);

	  void clearPendingInterruptsFlag(uint16_t interruptMask) const;
	  void initialise();
};

/*
 * Typedefs for easy use
 */

typedef CanInterruptFeature<1> Can1InterruptFeature;
typedef CanInterruptFeature<2> Can2InterruptFeature;


 /**
   * Template static data member initialisation
   */

  template<uint8_t TCANNumber>
  inline CanInterruptFeature<TCANNumber>::CanInterruptFeature(Can& can)
   : CanFeatureBase(can) {
	  _interruptMask=0;
	  _nvicPriority=_nvicSubPriority=0;
	  _canInstance=this;
  }


  /**
     * Destructor, if any interrupts are configured for this class then
     * disable them so that ISR calls don't disappear off into nothing
     */

    template<uint8_t TCANNumber>
    inline CanInterruptFeature<TCANNumber>::~CanInterruptFeature() {
      if(_interruptMask!=0)
        disableInterrupts(_interruptMask);
    }


    /**
       * Set the NVIC priority and subpriority numbers.  This function just remembers your
       * numbers. The actual setting of the priority happens in enableInterrupts()
       * @param priority
       * @param subPriority
       */

      template<uint8_t TCANNumber>
      inline void CanInterruptFeature<TCANNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
        _nvicPriority=priority;
        _nvicSubPriority=subPriority;
      }

      /**
        * Enable the interrupts specified in the mask
        * @param interruptMask The bitmask of interrupts, e.g. CAN_IT_TME / CAN_IT_FMP0 / CAN_IT_FF0
        */

       template<uint8_t TCANNumber>
       inline void CanInterruptFeature<TCANNumber>::enableInterrupts(uint16_t interruptMask) {

         _interruptMask|=interruptMask;
         CanInterruptFeatureEnabler<TCANNumber>::enable(_nvicPriority,_nvicSubPriority);

         CAN_ITConfig(_can,interruptMask,ENABLE);
       }

       /**
          * Disable the interrupts specified in the mask
          * @param interruptMask The bitmask of interrupts, e.g. I2C_IT_TXE / I2C_IT_RXNE
          */

         template<uint8_t TCANNumber>
         inline void CanInterruptFeature<TCANNumber>::disableInterrupts(uint16_t interruptMask) {
           _interruptMask&=~interruptMask;
           CAN_ITConfig(_can,interruptMask,DISABLE);
         }

         /**
            * Clear the pending bit(s) for interrupt
            * @param interruptMask The bitmask of interrupts, e.g. I2C_IT_TXE / I2C_IT_RXNE
            */

           template<uint8_t TCANNumber>
           inline void CanInterruptFeature<TCANNumber>::clearPendingInterruptsFlag(uint16_t interruptMask) const {
             CAN_ClearITPendingBit(_can,interruptMask);
           }
           template<uint8_t TCANNumber>
		    inline void CanInterruptFeature<TCANNumber>::initialise() {
           	  }


           /**
            * Enabler specialisation, Can 1
            */

           template<>
           inline void CanInterruptFeatureEnabler<1>::enable(uint8_t priority,uint8_t subPriority) {
             _forceLinkage=&USB_HP_CAN1_TX_IRQHandler;
             Nvic::configureIrq(USB_HP_CAN1_TX_IRQn,ENABLE,priority,subPriority);
             Nvic::configureIrq(USB_LP_CAN1_RX0_IRQn,ENABLE,priority,subPriority);
             Nvic::configureIrq(CAN1_RX1_IRQn,ENABLE,priority,subPriority);
			 Nvic::configureIrq(CAN1_SCE_IRQn,ENABLE,priority,subPriority);
           }



           /**
             * Enabler specialisation, Can 2
             */
           //TODO Need to do for Connectivity line
            template<>
            inline void CanInterruptFeatureEnabler<2>::enable(uint8_t priority,uint8_t subPriority) {
             _forceLinkage=&USB_HP_CAN1_TX_IRQHandler;
             Nvic::configureIrq(USB_HP_CAN1_TX_IRQn,ENABLE,priority,subPriority);
             Nvic::configureIrq(USB_LP_CAN1_RX0_IRQn,ENABLE,priority,subPriority);
             Nvic::configureIrq(CAN1_RX1_IRQn,ENABLE,priority,subPriority);
 			 Nvic::configureIrq(CAN1_SCE_IRQn,ENABLE,priority,subPriority);
            }



}
