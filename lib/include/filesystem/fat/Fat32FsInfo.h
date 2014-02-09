/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief FsInfo structure used to accelerate some file system operation.
     *
     * This is not supported in the stm32plus implementation.
     */

    struct Fat32FsInfo {

        /// Value 0x41615252. This lead signature is used to validate that this is in fact an FSInfo sector.
        uint32_t FSI_LeadSig;

        /// This field is currently reserved for future expansion. FAT32 format code should always initialize all bytes of this field to 0. Bytes in this field must currently never be used.
        uint8_t FSI_Reserved1[480];

        /// Value 0x61417272. Another signature that is more localized in the sector to the location of the fields that are used.
        uint32_t FSI_StrucSig;

        /// Contains the last known free cluster count on the volume. If the value is 0xFFFFFFFF, then the free count is unknown and must be computed. Any other value can be used, but is not necessarily correct. It should be range checked at least to make sure it is <= volume cluster count.
        uint32_t FSI_Free_Count;

        /// This is a hint for the FAT driver. It indicates the cluster number at which the driver should start looking for free clusters. Because a FAT32 FAT is large, it can be rather time consuming if there are a lot of allocated clusters at the start of the FAT and the driver starts looking for a free cluster starting at cluster 2. Typically this value is set to the last cluster number that the driver allocated. If the value is 0xFFFFFFFF, then there is no hint and the driver should start looking at cluster 2. Any other value can be used, but should be checked first to make sure it is a valid cluster number for the volume.
        uint32_t FSI_Nxt_Free;

        /// This field is currently reserved for future expansion. FAT32 format code should always initialize all bytes of this field to 0. Bytes in this field must currently never be used.
        uint8_t FSI_Reserved2[12];

        /// Value 0xAA550000. This trail signature is used to validate that this is in fact an FSInfo sector. Note that the high 2 bytes of this value—which go into the bytes at offsets 510 and 511—match the signature bytes used at the same offsets in sector 0.
        uint32_t FSI_TrailSig;

    } __attribute__ ((packed));
  }
}
