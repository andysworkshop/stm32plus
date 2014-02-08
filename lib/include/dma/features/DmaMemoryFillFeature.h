/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Do-nothing type to supply the correct template parameters to the base memory copy class
   * for filling memory with a constant value: the source address is not incremented.
   */

  template<uint32_t TDataSize=DMA_MemoryDataSize_Byte>
  using DmaMemoryFillFeature = DmaMemoryCopyBaseFeature<DMA_PeripheralInc_Disable,TDataSize>;
}
