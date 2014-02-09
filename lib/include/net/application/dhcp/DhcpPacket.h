/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Structure of a DHCP packet. This can be cast directly on to memory.
     */

    struct DhcpPacket {

      /**
       * DHCP operations
       */

      enum class Operation : uint8_t {
        REQUEST = 1,      //!< REQUEST
        REPLY = 2         //!< REPLY
      };


      /**
       * Hardware address types from the ARP section of RFC1700. We only use one
       * so that's all I'm including
       */

      enum class HardwareAddressType : uint8_t {
        ETHERNET_10Mb = 1               //!< ETHERNET_10Mb
      };


      /**
       * MessageType option values
       */

      enum class MessageType : uint8_t {
        NONE      = 0,    //!< not a real code
        DISCOVER  = 1,    //!< DISCOVER
        OFFER     = 2,    //!< OFFER
        REQUEST   = 3,    //!< REQUEST
        DECLINE   = 4,    //!< DECLINE
        ACK       = 5,    //!< ACK
        NAK       = 6,    //!< NAK
        RELEASE   = 7,    //!< RELEASE
        INFORM    = 8     //!< INFORM
      };

      /**
       * Flags
       */

      enum class Flags : uint16_t {
        BROADCAST_RESPONSE = 0x8000           ///< server must respond via a broadcast
      };


      /**
       * Structure members
       */

      Operation dhcp_op;                      ///< operation
      HardwareAddressType dhcp_htype;         ///< hardware address type
      uint8_t dhcp_hlen;                      ///< hardware address length (6 for a MAC)
      uint8_t dhcp_hops;
      uint32_t dhcp_xid;
      uint16_t dhcp_secs;
      uint16_t dhcp_flags;
      IpAddress dhcp_ciaddr;
      IpAddress dhcp_yiaddr;
      IpAddress dhcp_siaddr;
      IpAddress dhcp_giaddr;
      MacAddress dhcp_chaddr;               ///< 6 bytes of MAC address
      uint8_t dhcp_chaddr_[10];             ///< padding to make up the 16-byte field
      uint8_t dhcp_sname[64];
      uint8_t dhcp_file[128];
      uint32_t dhcp_magic;


      /**
       * Get the total packet size, including options
       * @return
       */

      static uint16_t getPacketSize() {
        return sizeof(DhcpPacket);
      }


      /**
       * Create options common to all packets
       * @param myMacAddress Local client MAC
       * @param transactionId 32-bit ID that will be carried through this transaction
       */

      void createCommonRequestOptions(const MacAddress& myMacAddress,uint32_t transactionId) {

        // the packet is mostly zeros

        memset(this,0,sizeof(DhcpPacket));

        // set up the non-zero common values

        dhcp_op=Operation::REQUEST;                       // uint8_t
        dhcp_htype=HardwareAddressType::ETHERNET_10Mb;      // uint8_t
        dhcp_hlen=6;                                        // uint8_t
        dhcp_chaddr=myMacAddress;                         // 6 bytes
        dhcp_xid=transactionId;                           // uint32_t
        dhcp_magic=NetUtil::htonl(0x63825363);            // uint32_t
      }


      /**
       * Create a DISCOVERY packet. This is the first one in the DHCP transaction
       * that is used to solicit offers from DHCP servers on the network
       * @param myMacAddress Our local MAC address
       * @param transactionId 32-bit ID that will be carried through this transaction
       * @param expectUnicastResponse true if the client wants a unicast response from the server
       * @param requestDefaultGateway true if we want to know the default gateway
       * @param requestSubnetMask true if we want to know the subnet mask
       * @param requestDnsServer true if we want to know the DNS servers
       * @param requestDomainName true if we want to know the domain suffix
       * @return The packet size with the options
       */

      uint16_t createDiscovery(const MacAddress& myMacAddress,
                               uint32_t transactionId,
                               bool expectUnicastResponse,
                               bool requestDefaultGateway,
                               bool requestSubnetMask,
                               bool requestDnsServer,
                               bool requestDomainName) {

        createCommonRequestOptions(myMacAddress,transactionId);

        if(!expectUnicastResponse)
          dhcp_flags=NetUtil::htons(static_cast<uint16_t>(Flags::BROADCAST_RESPONSE));

        uint8_t *options=reinterpret_cast<uint8_t *>(this)+getPacketSize();

        options=createMessageTypeOption(options,MessageType::DISCOVER);
        options=createParameterRequestListOption(options,requestDefaultGateway,requestSubnetMask,requestDnsServer,requestDomainName);
        options=createEndOption(options);

        return options-reinterpret_cast<uint8_t *>(this);
      }


      /**
       * Create MessageType option
       * @param options Start of options data
       * @param type MessageType
       * @return The next option start location
       */

      uint8_t *createMessageTypeOption(uint8_t *options,MessageType type) {

        *options++=53;
        *options++=1;
        *options++=static_cast<uint8_t>(type);

        return options;
      }


      /**
       * Create a REQUEST packet.
       * @param myMacAddress Our local mac adress
       * @param transactionId 32-bit ID that will be carried through this transaction
       * @param dhcpServerAddress Address of the DHCP server
       * @param offeredIpAddress Address offered to us in the OFFER response
       * @param hostname Our hostname that we report to the server
       * @param expectUnicastResponse true if the client wants a unicast response from the server
       * @param requestDefaultGateway true if we want to know the default gateway
       * @param requestSubnetMask true if we want to know the subnet mask
       * @param requestDnsServer true if we want to know the DNS servers
       * @param requestDomainName true if we want to know the domain suffix
       * @return The packet size with the options
       */

      uint16_t createRequest(const MacAddress& myMacAddress,
                             uint32_t transactionId,
                             const IpAddress& dhcpServerAddress,
                             const IpAddress& offeredIpAddress,
                             const char *hostname,
                             bool expectUnicastResponse,
                             bool requestDefaultGateway,
                             bool requestSubnetMask,
                             bool requestDnsServer,
                             bool requestDomainName) {

        createCommonRequestOptions(myMacAddress,transactionId);

        if(!expectUnicastResponse)
          dhcp_flags=NetUtil::htons(static_cast<uint16_t>(Flags::BROADCAST_RESPONSE));

        dhcp_siaddr=dhcpServerAddress;

        uint8_t *options=reinterpret_cast<uint8_t *>(this)+getPacketSize();

        options=createMessageTypeOption(options,MessageType::REQUEST);

        if(hostname)
          options=createHostNameOption(options,hostname);

        options=createRequestIpAddressOption(options,offeredIpAddress);
        options=createParameterRequestListOption(options,requestDefaultGateway,requestSubnetMask,requestDnsServer,requestDomainName);
        options=createDhcpServerOption(options,dhcpServerAddress);
        options=createEndOption(options);

        return options-reinterpret_cast<uint8_t *>(this);
      }


      /**
       * Create the hostname option (12)
       * @param options Start of options data
       * @param hostname My host name
       * @return The next option start location
       */

      uint8_t *createHostNameOption(uint8_t *options,const char *hostname) {

        uint8_t hostnamelen(strlen(hostname));

        *options++=12;
        *options++=hostnamelen;
        memcpy(options,hostname,hostnamelen);
        options+=hostnamelen;

        return options;
      }


      /**
       * Create the requested IP address option (50)
       * @param options Start of options data
       * @param ipAddress The requested IP address
       * @return The next option start location
       */

      uint8_t *createRequestIpAddressOption(uint8_t *options,const IpAddress& address) {

        *options++=50;
        *options++=4;
        *reinterpret_cast<uint32_t *>(options)=address.ipAddress;
        options+=4;

        return options;
      }


      /**
       * Create DHCP server option
       * @param options Start of options data
       * @param type MessageType
       * @return The next option start location
       */

      uint8_t *createDhcpServerOption(uint8_t *options,const IpAddress& dhcpServerAddress) {

        *options++=54;
        *options++=4;
        *reinterpret_cast<uint32_t *>(options)=dhcpServerAddress.ipAddress;

        return options+4;
      }


      /**
       * Create the additional request items option
       * @param options Start of options data
       * @param requestDefaultGateway
       * @param requestSubnetMask
       * @param requestDnsServer
       * @param requestDomainName
       * @return The next option start location
       */

      uint8_t *createParameterRequestListOption(uint8_t *options,
                                                bool requestDefaultGateway,
                                                bool requestSubnetMask,
                                                bool requestDnsServer,
                                                bool requestDomainName) {

        if(requestDefaultGateway || requestSubnetMask || requestDnsServer) {

          uint8_t *len;

          *options++=55;
          len=options++;
          *len=0;

          if(requestDefaultGateway) {
            (*len)++;
            *options++=3;
          }

          if(requestSubnetMask) {
            (*len)++;
            *options++=1;
          }

          if(requestDnsServer) {
            (*len)++;
            *options++=6;
          }

          if(requestDomainName) {
            (*len)++;
            *options++=15;
          }
        }

        return options;
      }


      /**
       * Create the end-of-options option
       * @param options Start of options data
       * @return The next option start location
       */

      uint8_t *createEndOption(uint8_t *options) {
        *options++=0xff;
        return options;
      }


      /**
       * Get the message type option
       * @return The message type
       */

      MessageType getMessageType() {

        uint8_t *option;

        if((option=getOption(53))==nullptr)
          return MessageType::NONE;

        return static_cast<MessageType>(option[2]);
      }


      /**
       * Get the address of a specific option
       * @return The address or nullptr if not found
       */

      uint8_t *getOption(uint8_t code) {

        uint8_t *options;
        options=reinterpret_cast<uint8_t *>(this)+getPacketSize();

        while(*options!=255 && *options!=code) {
          if(*options==0)             // code 0 is just a null byte
            options++;
          else
            options+=options[1]+2;    // length is in *(code+1)
        }

        return *options==255 ? nullptr : options;
      }
    } __attribute__((packed));
  }
}
