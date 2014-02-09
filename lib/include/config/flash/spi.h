/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the flash memory interfaces. Support is provided for the Atmel AT45DB161D
 * serial flash device and the Samsung K9F1G08Q0C NAND flash device.
 */

// spi flash depends on spi, timing, stream

#include "config/spi.h"
#include "config/timing.h"
#include "config/stream.h"

// include the main classes

#include "flash/spi/Base.h"
#include "flash/spi/Command.h"
#include "flash/spi/SpiFlashDevice.h"

// include the 'standard' commands

#include "flash/spi/commands/Commands.h"
#include "flash/spi/commands/WriteStatusRegister.h"
#include "flash/spi/commands/WriteEnable.h"
#include "flash/spi/commands/WriteDisable.h"
#include "flash/spi/commands/PageProgram.h"
#include "flash/spi/commands/ReadStatusRegister.h"
#include "flash/spi/commands/Read.h"
#include "flash/spi/commands/FastRead.h"
#include "flash/spi/commands/SectorErase.h"
#include "flash/spi/commands/BlockErase.h"
#include "flash/spi/commands/ChipErase.h"
#include "flash/spi/commands/ReadJedecId.h"
#include "flash/spi/commands/DeepPowerDown.h"
#include "flash/spi/commands/ReleaseDeepPowerDown.h"

// include the manufacturer devices

#include "flash/spi/devices/StandardSpiFlashDevice.h"

#include "flash/spi/devices/w25q16dw/Commands.h"
#include "flash/spi/devices/w25q16dw/EraseSuspend.h"
#include "flash/spi/devices/w25q16dw/EraseResume.h"
#include "flash/spi/devices/w25q16dw/W25Q16DW.h"

#include "flash/spi/devices/s25fl208k/Commands.h"
#include "flash/spi/devices/s25fl208k/S25FL208K.h"

// include the stream class

#include "flash/spi/SpiFlashInputStream.h"
