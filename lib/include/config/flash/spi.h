/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the flash memory interfaces. Support is provided for the Atmel AT45DB161D
 * serial flash device and the Samsung K9F1G08Q0C NAND flash device.
 */

// spi flash depends on spi

#include "config/spi.h"

// include the main classes

#include "flash/spi/Base.h"
#include "flash/spi/Command.h"

// include the 'standard' commands

#include "flash/spi/commands/WriteStatus.h"
#include "flash/spi/commands/WriteEnable.h"
#include "flash/spi/commands/WriteDisable.h"
#include "flash/spi/commands/PageProgram.h"
#include "flash/spi/commands/ReadStatusRegister.h"
#include "flash/spi/commands/Read.h"
#include "flash/spi/commands/SectorErase.h"

// include the manufacturer devices

#include "flash/spi/devices/W25Q16DW.h"
