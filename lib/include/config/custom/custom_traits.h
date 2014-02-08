/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/*
 default do-nothing customisation file intended as a placeholder when you do not
 create custom/custom_traits.h of your own

 custom/custom_traits.h should contain full specialisations of PeripheralTraits
 to match your app's requirements for each peripheral. e.g:

template<>
struct PeripheralTraits<PERIPHERAL_USART1> {
  enum { GPIO_SPEED = GPIO_Speed_10Mhz };
};

 The compilation option "-iquot ." ensures that your "custom/custom_traits.h" is
 picked up before this empty file provided as a fallback by the library.
*/
