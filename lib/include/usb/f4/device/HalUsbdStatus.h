/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Handle the case where a HAL status needs to be translated to a USBD status
     * HAL status in, USBD status out.
     */

    struct HalUsbStatus {

      USBD_StatusTypeDef status;

      /**
       * Constructor
       */

      HalUsbStatus()
        : status(USBD_OK) {
      }


      /**
       * Assign a new status
       * @param newStatus The new status
       * @return self reference
       */

      HalUsbStatus& operator=(HAL_StatusTypeDef newStatus) {

        // translate

        switch(newStatus) {

          case HAL_OK:
            status=USBD_OK;
            break;

          case HAL_BUSY:
            status=USBD_BUSY;
            break;

          default:
            status=USBD_FAIL;
            break;
        }

        return *this;
      }


      /**
       * Return the status
       */

      operator USBD_StatusTypeDef() const {
        return status;
      }
    };
  }
}
