/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the networking support. Support is provided for the
 * builtin MAC on the F4.
 */

#if defined(STM32PLUS_F4) || defined(STM32PLUS_F1_CL_E)


// net depends on GPIO, RCC, traits, timing, event, smart pointers, meta, stl slist, concurrent, rtc, string, rng, stream

#include "config/gpio.h"
#include "config/rcc.h"
#include "config/traits.h"
#include "config/timing.h"
#include "config/event.h"
#include "config/rtc.h"
#include "config/string.h"
#include "config/concurrent.h"
#include "config/rng.h"
#include "config/stream.h"
#include "memory/scoped_array.h"
#include "memory/circular_buffer.h"
#include "memory/scoped_ptr.h"
#include "memory/linked_ptr.h"
#include "util/Meta.h"
#include "iterator"
#include "slist"
#include "vector"
#include "list"
#include "algorithm"
#include "string"

// generic includes

#include "net/EtherType.h"
#include "net/NetUtil.h"
#include "net/datalink/DatalinkChecksum.h"
#include "net/NetBuffer.h"
#include "net/NetEventDescriptor.h"
#include "net/NetworkErrorEvent.h"
#include "net/NetworkEvents.h"
#include "net/NetworkDebugEvent.h"
#include "net/NetworkIntervalTicker.h"
#include "net/NetworkUtilityObjects.h"
#include "net/NetMeta.h"

#include "net/transport/icmp/IcmpType.h"
#include "net/transport/icmp/IcmpCode.h"

// physical layer

#include "net/physical/PhysicalLayer.h"
#include "net/physical/PhyReadRequestEvent.h"
#include "net/physical/PhyWriteRequestEvent.h"
#include "net/physical/PhyBase.h"
#include "net/physical/ksz8051mll/KSZ8051MLL.h"
#include "net/physical/ksz8091rna/KSZ8091RNA.h"
#include "net/physical/dp83848c/DP83848C.h"
#include "net/physical/lan8710a/LAN8710A.h"
#include "net/physical/PhyHardReset.h"

// data link layer

#include "net/datalink/DatalinkFrame.h"
#include "net/datalink/DatalinkFrameEvent.h"
#include "net/datalink/DatalinkFrameSentEvent.h"

#include "net/datalink/DatalinkLayer.h"

#include "net/datalink/mac/fwlib/ethernet.h"

#include "net/datalink/MacAddress.h"
#include "net/datalink/EthernetTransmitRequestEvent.h"
#include "net/datalink/EthernetFrameData.h"
#include "net/datalink/EthernetTaggedFrameData.h"
#include "net/datalink/EthernetSnapFrameData.h"
#include "net/datalink/EthernetTaggedSnapFrameData.h"
#include "net/datalink/EthernetFrame.h"
#include "net/datalink/mac/MacAddressFilter.h"
#include "net/datalink/mac/MacDefaultPinPackage.h"
#include "net/datalink/mac/MacBase.h"
#include "net/datalink/mac/Mac.h"

#if defined(STM32PLUS_F4)

  #include "net/datalink/mac/f4/MacRemapPinPackage.h"
  #include "net/datalink/mac/f4/MiiInterface.h"
  #include "net/datalink/mac/f4/RmiiInterface.h"

#elif defined(STM32PLUS_F1_CL_E)

  #include "net/datalink/mac/f1/MacRemapPinPackage.h"
  #include "net/datalink/mac/f1/MiiInterface.h"
  #include "net/datalink/mac/f1/RmiiInterface.h"

#else
  #error "Unsupported MCU"
#endif

// network layer

#include "net/network/ip/IpAddress.h"
#include "net/network/ip/IpSubnetMask.h"

#include "net/application/DomainNameAnnouncementEvent.h"
#include "net/application/IpAddressAnnouncementEvent.h"
#include "net/application/IpDefaultGatewayAnnouncementEvent.h"
#include "net/application/IpDnsServersAnnouncementEvent.h"
#include "net/application/IpSubnetMaskAnnouncementEvent.h"
#include "net/datalink/MacAddressAnnoucementEvent.h"

#include "net/network/IpProtocol.h"
#include "net/network/ip/InternetChecksum.h"
#include "net/network/ip/IpTransmitRequestEvent.h"
#include "net/network/arp/ArpMappingRequestEvent.h"

#include "net/network/ip/IpAddressMappingEvent.h"

#include "net/network/arp/ArpOperation.h"
#include "net/network/arp/ArpFrameData.h"
#include "net/network/arp/ArpCache.h"
#include "net/network/arp/ArpReceiveEvent.h"
#include "net/network/arp/Arp.h"

#include "net/network/ip/IpPorts.h"
#include "net/network/ip/IpPacketHeader.h"
#include "net/network/ip/IpPacket.h"
#include "net/network/ip/IpPacketEvent.h"
#include "net/network/ip/features/IpFragmentedPacket.h"
#include "net/network/ip/features/IpPacketReassemblerFeature.h"
#include "net/network/ip/features/IpPacketFragmentFeature.h"
#include "net/network/ip/features/IpDisablePacketFragmentFeature.h"
#include "net/network/ip/features/IpDisablePacketReassemblerFeature.h"
#include "net/network/ip/Ip.h"

#include "net/network/NetworkLayer.h"

// transport layer

#include "net/transport/icmp/IcmpPacket.h"
#include "net/transport/icmp/IcmpPacketEvent.h"
#include "net/transport/icmp/IcmpTransmitRequestEvent.h"
#include "net/transport/icmp/IcmpErrorPacket.h"
#include "net/transport/icmp/Icmp.h"

#include "net/transport/udp/UdpDatagram.h"
#include "net/transport/udp/UdpDatagramEvent.h"
#include "net/transport/udp/Udp.h"

#include "net/transport/tcp/TcpOptions.h"
#include "net/transport/tcp/TcpHeaderFlags.h"
#include "net/transport/tcp/TcpState.h"
#include "net/transport/tcp/TcpHeader.h"
#include "net/transport/tcp/TcpSegmentEvent.h"
#include "net/transport/tcp/TcpTransmitWindow.h"
#include "net/transport/tcp/TcpReceiveWindow.h"
#include "net/transport/tcp/TcpConnectionStateChangedEvent.h"
#include "net/transport/tcp/TcpConnectionState.h"
#include "net/transport/tcp/TcpClosingConnectionState.h"
#include "net/transport/tcp/TcpEvents.h"
#include "net/transport/tcp/TcpFindConnectionNotificationEvent.h"
#include "net/transport/tcp/TcpConnectionReleasedEvent.h"
#include "net/transport/tcp/TcpConnectionClosedEvent.h"
#include "net/transport/tcp/TcpConnectionDataReadyEvent.h"
#include "net/transport/tcp/TcpReceiveBuffer.h"
#include "net/transport/tcp/TcpConnection.h"
#include "net/transport/tcp/TcpClientConnection.h"
#include "net/transport/tcp/TcpResendDelayCalculator.h"
#include "net/transport/tcp/TcpAcceptEvent.h"
#include "net/transport/tcp/TcpServerReleasedEvent.h"
#include "net/transport/tcp/TcpServerBase.h"
#include "net/transport/tcp/TcpServer.h"
#include "net/transport/tcp/Tcp.h"
#include "net/transport/tcp/TcpWaitState.h"
#include "net/transport/tcp/TcpConnectionArray.h"
#include "net/transport/tcp/TcpTextLineReceiver.h"
#include "net/transport/tcp/TcpOutputStreamOfStreams.h"
#include "net/transport/tcp/TcpInputStream.h"
#include "net/transport/tcp/TcpOutputStream.h"
#include "net/transport/TransportLayer.h"

// application layer

#include "net/application/dns/DnsPacketHeader.h"
#include "net/application/dns/DnsQueryPacket.h"
#include "net/application/dns/DnsReplyPacket.h"
#include "net/application/dns/DnsCache.h"
#include "net/application/dns/Dns.h"

#include "net/application/dhcp/DhcpRenewalDueEvent.h"
#include "net/application/dhcp/DhcpPacket.h"
#include "net/application/dhcp/DhcpClient.h"
#include "net/application/staticIpClient/StaticIpClient.h"
#include "net/application/ping/Ping.h"

#include "net/application/llip/LinkLocalIp.h"

#include "net/application/ApplicationLayer.h"

// network stack

#include "net/NetworkStack.h"

#endif
