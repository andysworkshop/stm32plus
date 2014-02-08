/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Do-nothing type to supply the correct template parameters to the base memory copy class
   * for copying a memory block from: the source address is incremented.
   */

  template<uint32_t TDataSize=DMA_MemoryDataSize_Byte>
  using DmaMemoryCopyFeature = DmaMemoryCopyBaseFeature<DMA_PeripheralInc_Enable,TDataSize>;
}
