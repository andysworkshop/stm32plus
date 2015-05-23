/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Base class for all network events, contains an identifier that
     * can be used to determine how to downcast this structure
     */

    struct NetEventDescriptor {

        /**
         * Possible event types raised
         */

        enum class NetEventType {
          NETWORK_ERROR,                ///< An error occurred
          PHY_READ_REQUEST,             ///< request to read a PHY register
          PHY_WRITE_REQUEST,            ///< request to write a PHY register
          ETHERNET_TRANSMIT_REQUEST,    ///< request to send an ethernet frame
          DATALINK_FRAME,               ///< Datalink frame arrived
          DATALINK_FRAME_SENT,          ///< Datalink frame sent on to the wire
          IP_PACKET,                    ///< IP packet arrived
          IP_TRANSMIT_REQUEST,          ///< an IP packet needs to be transmitted
          IP_ADDRESS_MAPPING,         ///< an association between a MAC and an IP is being notified
          ICMP_PACKET,                  ///< an ICMP packet has been received
          ICMP_TRANSMIT_REQUEST,        ///< an ICMP packet should be sent
          UDP_DATAGRAM,                 ///< a UDP datagram has been received
          ARP_MAPPING_REQUEST,          ///< a request to get an ARP mapping
          ARP_FRAME,                    ///< an ARP frame has been received
          IP_ADDRESS_ANNOUNCEMENT,      ///< a new ip address has been assigned
          SUBNET_MASK_ANNOUNCEMENT,     ///< a new subnet mask has been assigned
          DOMAIN_NAME_ANNOUNCEMENT,     ///< a new domain name has been assigned
          DNS_SERVERS_ANNOUNCEMENT,     ///< DNS servers have been assigned
          DEFAULT_GATEWAY_ANNOUNCEMENT, ///< A new default gateway has been assigned
          MAC_ADDRESS_ANNOUNCEMENT,     ///< a new mac address has been set
          DHCP_RENEWAL_DUE,           ///< time to renew the DHCP lease
          TCP_ACCEPT,                   ///< TCP connection arrived
          TCP_SERVER_RELEASED,          ///< TCP server is being destroyed and a port can be released
          TCP_CONNECTION_RELEASED,      ///< A TCP connection is released
          TCP_SEGMENT,                  ///< TCP segment arrived
          TCP_FIND_CONNECTION,          ///< TCP connection finder
          TCP_CONNECTION_CLOSED,        ///< TCP remote end has closed
          TCP_CONNECTION_DATA_READY,    ///< we have buffered some data from the remote end
          TCP_CONNECTION_STATE_CHANGED, ///< the state of a TCP connection has changed
          DEBUG_MESSAGE                 ///< message for debugging
        };

        NetEventType eventType;

      public:
        NetEventDescriptor(NetEventType type)
          : eventType(type) {
        }
    };
  }
}
