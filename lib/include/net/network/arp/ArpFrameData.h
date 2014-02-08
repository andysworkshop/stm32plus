/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * ARP frame data, this can be cast on to a frame in memory. 28 byte packets.
     */

    struct ArpFrameData {

      uint16_t arp_hardwareType;
      uint16_t arp_protocolType;
      uint8_t arp_hardwareAddressLength;
      uint8_t arp_protocolAddressLength;
      ArpOperation arp_operation;
      MacAddress arp_senderHardwareAddress;
      IpAddress arp_senderProtocolAddress;
      MacAddress arp_targetHardwareAddress;
      IpAddress arp_targetProtocolAddress;


      /**
       * Constructor: create constant parts
       */

      ArpFrameData() {
        initialise();
      }


      /**
       * Set up the constant parts
       */

      void initialise() {
        arp_hardwareType=NetUtil::htons(1);         // 1 = ethernet
        arp_protocolType=NetUtil::htons(0x0800);      // 0x8000 = IP
        arp_hardwareAddressLength=6;                  // 48 bit MACs
        arp_protocolAddressLength=4;                  // 32 bit IP addresses
      }


      /**
       * Create a gratuitous ARP message (announcement)
       */

      void createGratuitous(const MacAddress& mac,const IpAddress& ip) {
        createMessage(mac,MacAddress::createBroadcastAddress(),ip,ip);
        arp_operation=ArpOperation::REQUEST;
      }


      /**
       * Create a reply message
       * @param request The request
       * @param replyMac The reply mac address
       * @param replyIp The reply IP address
       */

      void createReply(const ArpFrameData& request,const MacAddress& replyMac,const IpAddress& replyIp) {
        createMessage(replyMac,request.arp_senderHardwareAddress,replyIp,request.arp_senderProtocolAddress);
        arp_operation=ArpOperation::REPLY;
      }


      /**
       * Create a an ARP request message.
       * @param requestedIp The IP that we want to look up
       * @param myMacAddress our own MAC address
       * @param myIpAddress our own IP address
       */

      void createRequest(const IpAddress& requestedIp,const MacAddress& myMacAddress,const IpAddress& myIpAddress) {
        createMessage(myMacAddress,MacAddress(),myIpAddress,requestedIp);
        arp_operation=ArpOperation::REQUEST;
      }


      /**
       * Create an ARP message from parameters
       */

      void createMessage(const MacAddress& senderMac,const MacAddress& targetMac,
                         const IpAddress& senderIp,const IpAddress& targetIp) {

        arp_senderHardwareAddress=senderMac;
        arp_senderProtocolAddress=senderIp;
        arp_targetHardwareAddress=targetMac;
        arp_targetProtocolAddress=targetIp;
      }

    } __attribute__((packed));
  }
}
