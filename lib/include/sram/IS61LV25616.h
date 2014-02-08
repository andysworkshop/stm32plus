/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of the ISSI IS61LV25616 256K x 16 (4Mbit) SRAM
   */

  template<class TFsmc>
  class IS61LV25616 : public GenericSram<TFsmc> {

    public:

      /**
       * Access time of this device. Used for setting up the FSMC timing
       */

      enum AccessTime {
        /// 10ns
        TEN_NS,
        /// 12ns
        TWELVE_NS
      };

      /**
       * Size of the device, in bytes
       */

      enum {
        SIZE_IN_BYTES = 524288        // 4Mbit
      };

    protected:
      FsmcSramTiming createTiming(AccessTime accessTime);

    public:
      IS61LV25616(AccessTime accessTime);
      virtual ~IS61LV25616() {}

      // overrides from BlockDevice

      virtual uint32_t getTotalBlocksOnDevice() override;
  };


  /**
   * Constructor
   * @param bank Which bank it's attached to, e.g.  FSMC_Bank1_NORSRAM1
   * @param accessTime The access timing for this device, e.g. 10ns
   */

  template<class TFsmc>
  inline IS61LV25616<TFsmc>::IS61LV25616(AccessTime accessTime)
    : GenericSram<TFsmc>(createTiming(accessTime),17) {     // A17 is the last address line
  }


  /**
   * Create the timing structure
   * @return The timing structure
   */

  template<class TFsmc>
  inline FsmcSramTiming IS61LV25616<TFsmc>::createTiming(AccessTime accessTime) {

    FsmcSramTiming timing;

    timing.FSMC_AccessMode=FSMC_AccessMode_A;
    timing.FSMC_AddressHoldTime=0;
    timing.FSMC_BusTurnAroundDuration=0;
    timing.FSMC_CLKDivision=0;
    timing.FSMC_DataLatency=0;

    if(accessTime==TEN_NS) {
      timing.FSMC_AddressSetupTime=0;
      timing.FSMC_DataSetupTime=1;
    }
    else if(accessTime==TWELVE_NS) {
      timing.FSMC_AddressSetupTime=0;
      timing.FSMC_DataSetupTime=1;
    }

    return timing;
  }


  /**
   * Get the total number of blocks on this device = 524288/512 = 1024 blocks
   * @return 524288/512 = 1024 blocks
   */

  template<class TFsmc>
  inline uint32_t IS61LV25616<TFsmc>::getTotalBlocksOnDevice() {
    return 1024;
  }
}
