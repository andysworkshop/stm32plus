/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file is used to specify default 'traits' that are pervasive throughout a peripheral. A good example
 * is the GPIO speed (actually the rise and fall slope) which is set at a default of 50MHz.
 */


// generic implementation


#if defined(STM32PLUS_F1_HD)
  #include "traits/f1/hd/traits.h"
#elif defined(STM32PLUS_F1_CL_E)
  #include "traits/f1/cl/traits.h"
#elif defined(STM32PLUS_F1_MD)
  #include "traits/f1/md/traits.h"
#elif defined(STM32PLUS_F1_MD_VL)
  #include "traits/f1/mdvl/traits.h"
#elif defined(STM32PLUS_F4)
  #include "traits/f4/traits.h"
#elif defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)  || defined(STM32PLUS_F0_42)
  #include "traits/f0/traits.h"
#endif

/*
 * very important: the default custom_traits file does nothing. supply your
 * own config/custom/custom/traits.h file to override this and use the compiler
 * option -iquot <path> to ensure it gets picked up first. Use the content of this file to
 * provide your own specialisation of the PeripheralTraits structure for the peripheral
 * that you're interested in.
 */

#include "config/custom/custom_traits.h"
