/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Peripheral class for the hash processor. Needs to be parameterised with
   * the implementation of the hash function and an optional list of additional
   * features that you'd like to have baked in.
   *
   * For simplistic synchronous use there are typedefs such as SHA1HashPeripheral<>
   * that will get you a type that does SHA1 synchronously with no DMA and no
   * interrupts
   */

  template<class THashImpl,class... Features>
  struct HashPeripheral : THashImpl,Features... {
  };


  /**
   * Typedefs for the peripheral operating the available hash algorithms. Got to
   * love the C++0x 'template using' feature.
   */

  template<class... Features> using SHA1HashPeripheral=HashPeripheral<SHA1,Features...>;
}
