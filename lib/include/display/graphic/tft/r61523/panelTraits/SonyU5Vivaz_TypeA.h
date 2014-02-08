/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Panel traits for a Type A Sony U5 Vivaz
     *
     * This is the "original sony" panel that has all the correct manufacturer
     * values held in NVRAM and does not need runtime adjustment. Therefore
     * this class does not do anything.
     */

    struct SonyU5Vivaz_TypeA {
      template<class TAccessMode>
      static void initialise(TAccessMode& /* accessMode */) {
      }
    };
  }
}
