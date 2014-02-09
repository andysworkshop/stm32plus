/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Forward declaration of the function mapper template. The specialisations provide
   * the necessary level of abstraction, which on the F4 is only at the peripheral level.
   * Other MCUs (F3) need to be abstracted down to the pin level. The F1 does not have
   * any advanced mapping support so we just include a dummy implementation so that
   * the call signatures to the GpioPinInitialiser methods are compatible with all MCUs
   */

  template<PeripheralName TPeripheralName,uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper;
}
