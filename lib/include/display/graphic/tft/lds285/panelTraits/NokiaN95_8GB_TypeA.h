/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Panel traits for a Type A Nokia N95 8Gb. The type A screen does not
     * flip column and row addresses in portrait mode
     */

    class NokiaN95_8GB_TypeA {
      public:
        template<Orientation TOrientation>
        constexpr static uint8_t getColumnAddressCommand();

        template<Orientation TOrientation>
        constexpr static uint8_t getRowAddressCommand();
    };


    /**
     * Get the command used to set column address in portrait mode
     * @return lds285::COLUMN_ADDRESS_SET
     */

    template<>
    constexpr inline uint8_t NokiaN95_8GB_TypeA::getColumnAddressCommand<PORTRAIT>() {
      return lds285::COLUMN_ADDRESS_SET;
    }


    /**
     * Get the command used to set row address in portrait mode
     * @return lds285::ROW_ADDRESS_SET
     */

    template<>
    constexpr inline uint8_t NokiaN95_8GB_TypeA::getRowAddressCommand<PORTRAIT>() {
      return lds285::ROW_ADDRESS_SET;
    }
  }
}
