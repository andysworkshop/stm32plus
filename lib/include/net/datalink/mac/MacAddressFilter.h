/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * MAC address filtering option. Provides access to the STM32 built-in filtering
     * capabilities. You can whitelist or blacklist up to 3 MAC addresses as sources
     * or destinations.
     */

    template<class TPhysicalLayer>
    class MacAddressFilter : public virtual TPhysicalLayer {

      public:

        /**
         * Parameters class
         */

        struct Parameters {

          struct Filter {
            MacAddress address;           ///< address for filtering
            bool source;                  ///< true to filter sources (senders), false to filter destinations (default is true)
            uint32_t maskBytes;           ///< combination of ETH_MAC_AddressMask_Byte1..6. When set, the address byte for that bit is NOT used in the comparison. Default is zero.

            Filter() {
              source=true;
              maskBytes=0;
            }
          };

          Filter macfilter_filters[3];    ///< the addresses to filter on, up to 3.
          bool macfilter_whitelist;       ///< true to pass addresses that match, false to fail. default is true


          /**
           * Constructor
           */

          Parameters() {
            macfilter_whitelist=true;
          }
        };

      protected:

        void initialiseFilter(const typename Parameters::Filter& filter,uint32_t macId,uint8_t& sources,uint8_t& dests) const;

        bool initialise(const Parameters& params);
        bool startup();
    };


    /**
     * Initialise the filtering
     */

    template<class TPhysicalLayer>
    inline bool MacAddressFilter<TPhysicalLayer>::initialise(const Parameters& params) {

      uint8_t sources,dests;

      // initialise each filter

      sources=dests=0;

      initialiseFilter(params.macfilter_filters[0],ETH_MAC_Address1,sources,dests);
      initialiseFilter(params.macfilter_filters[1],ETH_MAC_Address2,sources,dests);
      initialiseFilter(params.macfilter_filters[2],ETH_MAC_Address3,sources,dests);

      // clear source/dest filtering options

      ETH->MACFFR&=~(0x300 | 8);

      // source filtering can be disabled, normal, inverse

      ETH->MACFFR|=sources==0 ?
                      ETH_SourceAddrFilter_Disable :
                      (params.macfilter_whitelist ? ETH_SourceAddrFilter_Normal_Enable : ETH_SourceAddrFilter_Inverse_Enable);


      // destination filtering can be normal, inverse

      ETH->MACFFR|=dests==0 || params.macfilter_whitelist ?
                      ETH_DestinationAddrFilter_Normal : ETH_DestinationAddrFilter_Inverse;

      return true;
    }


    /**
     * Startup, nothing to do
     * @return true
     */

    template<class TPhysicalLayer>
    inline bool MacAddressFilter<TPhysicalLayer>::startup() {
      return true;
    }


    /**
     * Initialise filtering for a given address
     * @param filter The filter definition
     * @param macId The mac address identifier (ETH_MAC_Address1..3)
     */

    template<class TPhysicalLayer>
    inline void MacAddressFilter<TPhysicalLayer>::initialiseFilter(const typename Parameters::Filter& filter,uint32_t macId,uint8_t& sources,uint8_t& dests) const {

      if(filter.address.isValid()) {

        // set the source/dest selection, filter mask bytes and enable it

        if(filter.source) {
          sources++;
          ETH_MACAddressFilterConfig(macId,ETH_MAC_AddressFilter_SA);
        }
        else {
          dests++;
          ETH_MACAddressFilterConfig(macId,ETH_MAC_AddressFilter_DA);
        }

        ETH_MACAddressMaskBytesFilterConfig(macId,filter.maskBytes);
        ETH_MACAddressPerfectFilterCmd(macId,ENABLE);
      }
    }
  }
}
