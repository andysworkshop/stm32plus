/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/device.h"


namespace stm32plus {

  /**
   * Get the Master Boot Record (MBR) from devices that support it.
   * Calls down to the subclass to read block zero from the device.
   *
   * @param[in] mbr_ Pointer to a caller-owned Mbr structure that this
   *  method will populate.
   *
   * @return false indicates failure and the error provider is populated.
   */

  bool BlockDevice::getMbr(Mbr* mbr_) {
    // check that we support an MBR

    if(getFormatType()!=formatMbr)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BLOCK_DEVICE,E_NO_MBR);

    // read it from the first sector in the device

    return readBlock(mbr_,0);
  }

}
