/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Template class for the MAC. Although the PHY is generally considered to be in
     * the same network layer (#2, datalink) as the MAC they are not independent devices.
     * The MAC is wholly dependent on, and strongly linked to its PHY. Therefore we implement
     * the PHY as a base of the MAC.
     * @tparam TPhy The type of PHY, e.g. KSZ8051MLL
     * @tparam TInterfaceMode The type of interface MiiInterface or RmiiInterface
     */

    template<class TPhysicalLayer>
    class Mac : public virtual TPhysicalLayer,
                public MacBase {

      public:

        /**
         * Parameters class for this MAC. This inherits from the base parameters so we can
         * pass them upwards. We also inherit from the stdperiph ETH_InitTypeDef
         * to avoid having to duplicate all the members.
         */

        struct Parameters : ETH_InitTypeDef,
                            virtual PhyBase::Parameters,
                            MacBase::Parameters {

          uint8_t mac_irqPriority;          //!< NVIC priority (default 0)
          uint8_t mac_irqSubPriority;       //!< NVIC sub-priority (default 0)


          /**
           * Constructor: initialise the stdperiph struct to default values
           */

          Parameters() {

            // default IRQ priority/subpriority

            mac_irqPriority=0;
            mac_irqSubPriority=0;

            // set defaults for the ST driver

            ETH_StructInit(this);

            // setup as if we're to be used by an IP client with auto-checksum offload

            ETH_AutoNegotiation=ETH_AutoNegotiation_Enable;
            ETH_LoopbackMode=ETH_LoopbackMode_Disable;
            ETH_RetryTransmission=ETH_RetryTransmission_Disable;
            ETH_AutomaticPadCRCStrip=ETH_AutomaticPadCRCStrip_Disable;
            ETH_ReceiveAll=ETH_ReceiveAll_Disable;
            ETH_BroadcastFramesReception=ETH_BroadcastFramesReception_Enable;
            ETH_PromiscuousMode=ETH_PromiscuousMode_Disable;
            ETH_MulticastFramesFilter=ETH_MulticastFramesFilter_Perfect;
            ETH_UnicastFramesFilter=ETH_UnicastFramesFilter_Perfect;
            ETH_ChecksumOffload=ETH_ChecksumOffload_Enable;

            ETH_DropTCPIPChecksumErrorFrame=ETH_DropTCPIPChecksumErrorFrame_Enable;
            ETH_ReceiveStoreForward=ETH_ReceiveStoreForward_Enable;
            ETH_TransmitStoreForward=ETH_TransmitStoreForward_Enable;

            ETH_ForwardErrorFrames=ETH_ForwardErrorFrames_Disable;
            ETH_ForwardUndersizedGoodFrames=ETH_ForwardUndersizedGoodFrames_Disable;
            ETH_SecondFrameOperate=ETH_SecondFrameOperate_Enable;
            ETH_AddressAlignedBeats=ETH_AddressAlignedBeats_Enable;
            ETH_FixedBurst=ETH_FixedBurst_Enable;
            ETH_RxDMABurstLength=ETH_RxDMABurstLength_32Beat;
            ETH_TxDMABurstLength=ETH_TxDMABurstLength_32Beat;

            // enable 1:1 (equal) access to the AHB bus by DMA. We receive one frame
            // completely out of the MAC FIFO and that frame can result in an auto-generated
            // response (e.g. ICMP reply). If access was not even then a high frequency of
            // such incoming frames could cause the TX queue to overflow.

            ETH_DMAArbitration=ETH_DMAArbitration_RoundRobin_RxTx_1_1;

            // don't call ETH_Start here because we still need to initialise the rest
            // of the stack and a receive interrupt may fire while we're doing that
          }
        };

      protected:
        bool initMac(Parameters& params);

        void onNotification(NetEventDescriptor& ned);

      public:
        bool initialise(Parameters& params);
        bool startup();

        bool phyReadRegister(uint16_t phyAddress,uint16_t regNumber,uint16_t& value,uint16_t timeoutMillis);
        bool phyWriteRegister(uint16_t phyAddress,uint16_t regNumber,uint16_t value,uint16_t timeoutMillis);
    };


    /**
     * Initialiser. Set up the MAC and the link
     * @param params The parameters class
     * @return true if all OK
     */

    template<class TPhysicalLayer>
    inline bool Mac<TPhysicalLayer>::initialise(Parameters& params) {

      // subscribe to notification events

      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Mac<TPhysicalLayer>::onNotification));

      // enable the interrupts

      Nvic::configureIrq(ETH_IRQn,ENABLE,params.mac_irqPriority,params.mac_irqSubPriority);

      // de-initialise the peripheral

      ETH_DeInit();

      // software reset and wait until done

      ETH_SoftwareReset();
      while(ETH_GetSoftwareResetStatus()==SET);

      // initialise the MAC. the cable must be connected and the network "live"
      // the PHY feature base class is configured into our hierarchy

      if(!initMac(params))
        return false;

      // enable all interrupts

      ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_AIS | ETH_DMA_IT_R | ETH_DMA_IT_T, ENABLE);

      // call upwards

      if(!MacBase::initialise(params))
        return false;

      // finished

      return true;
    }


    /**
     * Start Tx/Rx
     * @return
     */

    template<class TPhysicalLayer>
    inline bool Mac<TPhysicalLayer>::startup() {

      // call the base class

      if(!MacBase::startup())
        return false;

      // startup the MAC

      ETH_Start();
      return true;
    }


    /**
     * Network notification event handler
     * @param ned The event descriptor
     */

    template<class TPhysicalLayer>
    __attribute__((noinline)) inline void Mac<TPhysicalLayer>::onNotification(NetEventDescriptor& ned) {

      if(ned.eventType==NetEventDescriptor::NetEventType::PHY_READ_REQUEST) {

        PhyReadRequestEvent& revent(static_cast<PhyReadRequestEvent&>(ned));
        revent.succeeded=phyReadRegister(revent.phyAddress,revent.regNumber,revent.result,revent.timeoutMillis);
      }
      else if(ned.eventType==NetEventDescriptor::NetEventType::PHY_WRITE_REQUEST) {

        PhyWriteRequestEvent& wevent(static_cast<PhyWriteRequestEvent&>(ned));
        wevent.succeeded=phyWriteRegister(wevent.phyAddress,wevent.regNumber,wevent.regValue,wevent.timeoutMillis);
      }
    }


    /**
     * Initialise the MAC and the PHY. If this function succeeds then the link
     * has been established. This is largely cribbed from the ST example ETH_Init()
     * call in the sample ethernet driver code.
     * @return true if it works
     */

    template<class TPhysicalLayer>
    inline bool Mac<TPhysicalLayer>::initMac(Parameters& params) {

      uint32_t tmpreg;
      RCC_ClocksTypeDef rcc_clocks;
      uint32_t hclk;

      // Get the ETHERNET MACMIIAR value
      tmpreg=ETH->MACMIIAR;

      // Clear CSR Clock Range CR[2:0] bits
      tmpreg &= MACMIIAR_CR_MASK;

      // Get hclk frequency value
      RCC_GetClocksFreq(&rcc_clocks);
      hclk=rcc_clocks.HCLK_Frequency;

      // Set CR bits depending on hclk value
      if ((hclk >= 20000000) && (hclk < 35000000)) {
        // CSR Clock Range between 20-35 MHz
        tmpreg |= (uint32_t) ETH_MACMIIAR_CR_Div16;
      } else if ((hclk >= 35000000) && (hclk < 60000000)) {
        // CSR Clock Range between 35-60 MHz
        tmpreg |= (uint32_t) ETH_MACMIIAR_CR_Div26;
      } else if ((hclk >= 60000000) && (hclk < 100000000)) {
        // CSR Clock Range between 60-100 MHz
        tmpreg |= (uint32_t) ETH_MACMIIAR_CR_Div42;
      } else if ((hclk >= 100000000) && (hclk < 150000000)) {
        // CSR Clock Range between 100-150 MHz
        tmpreg |= (uint32_t) ETH_MACMIIAR_CR_Div62;         // only on the F4
      } else {
        // ((hclk >= 150000000)&&(hclk <= 168000000))
        // CSR Clock Range between 150-168 MHz
        tmpreg |= (uint32_t) ETH_MACMIIAR_CR_Div102;        // only on the F4
      }

      // Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range
      ETH->MACMIIAR=(uint32_t) tmpreg;

      /*
       * PHY initialization and configuration
       */

      // reset the PHY

      if(!this->phySoftReset())
        return false;

      if(params.ETH_AutoNegotiation!=ETH_AutoNegotiation_Disable) {

        // wait for link

        if(!this->phyWaitForLink())
          return false;

        // enable auto-negotiation

        if(!this->phyEnableAutoNegotiation())
          return false;

        // wait for auto-negotiation to completed

        if(!this->phyWaitForAutoNegotiationComplete())
          return false;

        // set duplex and speed

        bool value;
        if(!this->phyIsFullDuplex(value))
          return false;

        params.ETH_Mode=value ? ETH_Mode_FullDuplex : ETH_Mode_HalfDuplex;

        if(!this->phyIs100M(value))
          return false;

        params.ETH_Speed=value ? ETH_Speed_100M : ETH_Speed_10M;
      }
      else {

        // manual configuration of speed and mode

        if (params.ETH_Mode==ETH_Mode_FullDuplex)
          this->phySetFullDuplex();
        else
          this->phySetHalfDuplex();

        if (params.ETH_Speed==ETH_Speed_100M)
          this->phySet100M();
        else
          this->phySet10M();

        // Delay to assure PHY configuration
        this->phyPostConfigurationDelay();
      }


      /*
       * ETHERNET MACCR Configuration
       * */

      // Get the ETHERNET MACCR value
      tmpreg=ETH->MACCR;

      // Clear WD, PCE, PS, TE and RE bits
      tmpreg &= MACCR_CLEAR_MASK;

      // Set the WD bit according to ETH_Watchdog value
      // Set the JD: bit according to ETH_Jabber value
      // Set the IFG bit according to ETH_InterFrameGap value
      // Set the DCRS bit according to ETH_CarrierSense value
      // Set the FES bit according to ETH_Speed value
      // Set the DO bit according to ETH_ReceiveOwn value
      // Set the LM bit according to ETH_LoopbackMode value
      // Set the DM bit according to ETH_Mode value
      // Set the IPCO bit according to ETH_ChecksumOffload value
      // Set the DR bit according to ETH_RetryTransmission value
      // Set the ACS bit according to ETH_AutomaticPadCRCStrip value
      // Set the BL bit according to ETH_BackOffLimit value
      // Set the DC bit according to ETH_DeferralCheck value

      tmpreg|=(uint32_t) (params.ETH_Watchdog
          | params.ETH_Jabber | params.ETH_InterFrameGap
          | params.ETH_CarrierSense | params.ETH_Speed
          | params.ETH_ReceiveOwn | params.ETH_LoopbackMode
          | params.ETH_Mode | params.ETH_ChecksumOffload
          | params.ETH_RetryTransmission
          | params.ETH_AutomaticPadCRCStrip
          | params.ETH_BackOffLimit | params.ETH_DeferralCheck);
      // Write to ETHERNET MACCR
      ETH->MACCR=(uint32_t) tmpreg;

      /*
       * ETHERNET MACFFR Configuration
       */

      // Set the RA bit according to ETH_ReceiveAll value
      // Set the SAF and SAIF bits according to ETH_SourceAddrFilter value
      // Set the PCF bit according to ETH_PassControlFrames value
      // Set the DBF bit according to ETH_BroadcastFramesReception value
      // Set the DAIF bit according to ETH_DestinationAddrFilter value
      // Set the PR bit according to ETH_PromiscuousMode value
      // Set the PM, HMC and HPF bits according to ETH_MulticastFramesFilter value
      // Set the HUC and HPF bits according to ETH_UnicastFramesFilter value
      // Write to ETHERNET MACFFR
      ETH->MACFFR=(uint32_t) (params.ETH_ReceiveAll
          | params.ETH_SourceAddrFilter
          | params.ETH_PassControlFrames
          | params.ETH_BroadcastFramesReception
          | params.ETH_DestinationAddrFilter
          | params.ETH_PromiscuousMode
          | params.ETH_MulticastFramesFilter
          | params.ETH_UnicastFramesFilter);

      /*
       * ETHERNET MACHTHR and MACHTLR Configuration
       */

      // Write to ETHERNET MACHTHR
      ETH->MACHTHR=(uint32_t) params.ETH_HashTableHigh;
      // Write to ETHERNET MACHTLR
      ETH->MACHTLR=(uint32_t) params.ETH_HashTableLow;
      /*----------------------- ETHERNET MACFCR Configuration --------------------*/
      // Get the ETHERNET MACFCR value
      tmpreg=ETH->MACFCR;
      // Clear xx bits
      tmpreg &= MACFCR_CLEAR_MASK;

      // Set the PT bit according to ETH_PauseTime value
      // Set the DZPQ bit according to ETH_ZeroQuantaPause value
      // Set the PLT bit according to ETH_PauseLowThreshold value
      // Set the UP bit according to ETH_UnicastPauseFrameDetect value
      // Set the RFE bit according to ETH_ReceiveFlowControl value
      // Set the TFE bit according to ETH_TransmitFlowControl value
      tmpreg |= (uint32_t) ((params.ETH_PauseTime << 16)
          | params.ETH_ZeroQuantaPause
          | params.ETH_PauseLowThreshold
          | params.ETH_UnicastPauseFrameDetect
          | params.ETH_ReceiveFlowControl
          | params.ETH_TransmitFlowControl);
      // Write to ETHERNET MACFCR
      ETH->MACFCR=(uint32_t) tmpreg;

      /*
       * ETHERNET MACVLANTR Configuration
       */

      // Set the ETV bit according to ETH_VLANTagComparison value
      // Set the VL bit according to ETH_VLANTagIdentifier value
      ETH->MACVLANTR=(uint32_t) (params.ETH_VLANTagComparison
          | params.ETH_VLANTagIdentifier);

      /*
       * ETHERNET DMAOMR Configuration
       */

      // Get the ETHERNET DMAOMR value
      tmpreg=ETH->DMAOMR;
      // Clear xx bits
      tmpreg &= DMAOMR_CLEAR_MASK;

      // Set the DT bit according to ETH_DropTCPIPChecksumErrorFrame value
      // Set the RSF bit according to ETH_ReceiveStoreForward value
      // Set the DFF bit according to ETH_FlushReceivedFrame value
      // Set the TSF bit according to ETH_TransmitStoreForward value
      // Set the TTC bit according to ETH_TransmitThresholdControl value
      // Set the FEF bit according to ETH_ForwardErrorFrames value
      // Set the FUF bit according to ETH_ForwardUndersizedGoodFrames value
      // Set the RTC bit according to ETH_ReceiveThresholdControl value
      // Set the OSF bit according to ETH_SecondFrameOperate value
      tmpreg |= (uint32_t) (params.ETH_DropTCPIPChecksumErrorFrame
          | params.ETH_ReceiveStoreForward
          | params.ETH_FlushReceivedFrame
          | params.ETH_TransmitStoreForward
          | params.ETH_TransmitThresholdControl
          | params.ETH_ForwardErrorFrames
          | params.ETH_ForwardUndersizedGoodFrames
          | params.ETH_ReceiveThresholdControl
          | params.ETH_SecondFrameOperate);
      // Write to ETHERNET DMAOMR
      ETH->DMAOMR=(uint32_t) tmpreg;

      /*
       * ETHERNET DMABMR Configuration
       */

      // Set the AAL bit according to ETH_AddressAlignedBeats value
      // Set the FB bit according to ETH_FixedBurst value
      // Set the RPBL and 4*PBL bits according to ETH_RxDMABurstLength value
      // Set the PBL and 4*PBL bits according to ETH_TxDMABurstLength value
      // Set the DSL bit according to ETH_DesciptorSkipLength value
      // Set the PR and DA bits according to ETH_DMAArbitration value
      ETH->DMABMR=(uint32_t) (params.ETH_AddressAlignedBeats
          | params.ETH_FixedBurst | params.ETH_RxDMABurstLength
          | // !! if 4xPBL is selected for Tx or Rx it is applied for the other
          params.ETH_TxDMABurstLength
          | (params.ETH_DescriptorSkipLength << 2)
          | params.ETH_DMAArbitration | ETH_DMABMR_USP); // Enable use of separate PBL for Rx and Tx

      // Enable the Enhanced DMA descriptors if we're on the F4

#if defined(STM32PLUS_F4)
      ETH->DMABMR |= ETH_DMABMR_EDE;
#endif
      // Return Ethernet configuration success
      return true;
    }


    /**
     * Read a PHY register. This is derived from the similar function in the ST
     * driver with changes to support accurate timing. Can hang forever if called
     * from an IRQ and there's an error reading from the PHY.
     * @param phyAddress The station number
     * @param regNumber The register number
     * @param value[out] Where to store the read value
     * @param true if it works, false if it fails
     */

    template<class TPhysicalLayer>
    inline bool Mac<TPhysicalLayer>::phyReadRegister(uint16_t phyAddress,
                                                     uint16_t regNumber,
                                                     uint16_t& value,
                                                     uint16_t timeoutMillis) {

      volatile uint32_t tmpreg;
      uint32_t start;

      // get the ETHERNET MACMIIAR value
      tmpreg=ETH->MACMIIAR;

      // keep only the CSR Clock Range CR[2:0] bits value
      tmpreg &= ~MACMIIAR_CR_MASK;

      // prepare the MII address register value

      tmpreg |=(((uint32_t)phyAddress<<11) & ETH_MACMIIAR_PA); // Set the PHY device address
      tmpreg |=(((uint32_t)regNumber<<6) & ETH_MACMIIAR_MR);      // Set the PHY register address
      tmpreg &= ~ETH_MACMIIAR_MW;                              // Set the read mode
      tmpreg |= ETH_MACMIIAR_MB;                               // Set the MII Busy bit

      // write the result value into the MII Address register
      ETH->MACMIIAR=tmpreg;

      // check for the Busy flag

      start=MillisecondTimer::millis();

      do
      {
        if(MillisecondTimer::hasTimedOut(start,timeoutMillis))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_PHY_READ_TIMEOUT);

        tmpreg=ETH->MACMIIAR;
      } while((tmpreg & ETH_MACMIIAR_MB)!=0);

      // return data register value

      value=ETH->MACMIIDR;
      return true;
    }


    /**
     * Write a PHY register. This is derived from the similar function in the ST
     * driver with changes to support accurate timing. Can hang forever if called
     * from an IRQ and there's an error reading from the PHY.
     * @param phyAddress The station number
     * @param regNumber The register number
     * @param value[out] Where to store the read value
     * @param true if it works, false if it fails
     */

    template<class TPhysicalLayer>
    inline bool Mac<TPhysicalLayer>::phyWriteRegister(uint16_t phyAddress,
                                                      uint16_t regNumber,
                                                      uint16_t value,
                                                      uint16_t timeoutMillis) {
      volatile uint32_t tmpreg;
      uint32_t start;

      // get the ETHERNET MACMIIAR value
      tmpreg=ETH->MACMIIAR;

      // Keep only the CSR Clock Range CR[2:0] bits value
      tmpreg &= ~MACMIIAR_CR_MASK;

      // Prepare the MII register address value
      tmpreg |= (((uint32_t)phyAddress<<11) & ETH_MACMIIAR_PA);   // Set the PHY device address
      tmpreg |= (((uint32_t)regNumber<<6) & ETH_MACMIIAR_MR);     // Set the PHY register address
      tmpreg |= ETH_MACMIIAR_MW;                                  // Set the write mode
      tmpreg |= ETH_MACMIIAR_MB;                                  // Set the MII Busy bit

      // Give the value to the MII data register
      ETH->MACMIIDR=value;

      // Write the result value into the MII Address register
      ETH->MACMIIAR=tmpreg;

      // Check for the Busy flag

      start=MillisecondTimer::millis();

      do
      {
        if(MillisecondTimer::hasTimedOut(start,timeoutMillis))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_PHY_WRITE_TIMEOUT);

        tmpreg=ETH->MACMIIAR;
      } while((tmpreg & ETH_MACMIIAR_MB)!=0);

      return true;
    }
  }
}
