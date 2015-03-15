/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * Initialise the class
     * @param params The IP parameters class that holds the limits
     * @param utilityObjects The network utilities
     * @return true;
     */

    bool IpPacketReassemblerFeature::initialise(const Parameters& params,NetworkUtilityObjects& utilityObjects) {

      // save variables

      _params=params;
      _totalFragmentSize=0;
      _inFlightPacketCount=0;
      _utilityObjects=&utilityObjects;

      // subscribe to ticks for the expiry

      utilityObjects.subscribeIntervalTicks(
          _params.ip_fragmentExpiryIntervalCheckSeconds,
          NetworkIntervalTicker::TickIntervalSlotType::bind(this,&IpPacketReassemblerFeature::expireOldEntries)
        );

      return true;
    }


    /**
     * Startup (does nothing)
     */

    bool IpPacketReassemblerFeature::startup() {
      return true;
    }


    /**
     * Destructor, free all
     */

    IpPacketReassemblerFeature::~IpPacketReassemblerFeature() {

      // ensure we cannot be interrupted here

      IrqSuspend suspender;

      // clear the list

      while(!_frags.empty()) {
        delete _frags.front();
        _frags.pop_front();
      }
    }


    /**
     * Handle a packet fragment from the Ip class
     * @param[in] packet The packet fragment class
     * @param[out] fp points to the FragmentedPacket class that we're operating on
     * @return true if it worked
     */

    bool IpPacketReassemblerFeature::ip_handleFragment(const IpPacket& packet,IpFragmentedPacket*& fp) {

      // ensure we cannot be interrupted

      IrqSuspend suspend;
      return internalHandleFragment(packet,fp);
    }


    /*
     * Handle the reassembly under the protection of the critical section
     */

    bool IpPacketReassemblerFeature::internalHandleFragment(const IpPacket& packet,IpFragmentedPacket*& fp) {

      uint16_t offset,extendedLength,extra;
      IpFragmentedPacket::PacketId pid;

      // get the packet identifier. the combination of id, source address,
      // destination address and protocol identify a packet on the network

      pid.identification=packet.getIdentifier();
      pid.sourceAddress=packet.header->ip_sourceAddress;
      pid.destinationAddress=packet.header->ip_destinationAddress;
      pid.protocol=packet.header->ip_hdr_protocol;

      // find the existing fragment or create a new one

      if((fp=findFragment(pid))==nullptr)
        if(!createNewFragment(pid,fp))
          return false;

      // update the expiry time

      fp->expiryTime=_utilityObjects->getRtc().getTick()+_params.ip_fragmentExpirySeconds;

      // get the fragment offset

      offset=packet.getFragmentOffset();
      extendedLength=offset+packet.payloadLength;

      // if the memory buffer holding the packet being assembled is too small
      // then increase it

      if(extendedLength>fp->packetLength) {

        // can we handle it based on the max packet length restriction

        if(extendedLength>_params.ip_maxPacketLength) {
          ip_freePacket(fp);
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER,E_PACKET_TOO_BIG);
        }

        // can we handle it based on the overal memory usage cap

        if(_totalFragmentSize-fp->packetLength+extendedLength>_params.ip_maxFragmentedPacketMemoryUsage) {
          ip_freePacket(fp);
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER,E_MEMORY_LIMIT_EXCEEDED);
        }

        // if this is not the last fragment then we allocate an extra 8 bytes on the end for
        // the hole descriptor that will inevitably be required to describe the packet fragment
        // that must eventually land there.

        extra=packet.isLastFragment() ? 0 : 8;

        // increase the size of the fragment

        if((fp->packet=reinterpret_cast<uint8_t *>(realloc(fp->packet,extendedLength+extra)))==nullptr) {
          ip_freePacket(fp);
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER,E_OUT_OF_MEMORY);
        }

        // update the new total size (take off the old known size and add back on the new known size)

        _totalFragmentSize=_totalFragmentSize-fp->packetLength+extendedLength;

        // set the new fragment length

        fp->packetLength=extendedLength;
      }

      // deal with the hole list

      fp->handleHoleList(packet);

      // copy in the new fragment and return. the caller will check for nullptr in fp->firstHole
      // to indicate that the fragment is complete

      memcpy(&fp->packet[offset],packet.payload,packet.payloadLength);
      return true;
    }


    /**
     * Expire old entries that have timed out. Always called on the RTC tick IRQ handler. Never
     * call this on non-IRQ code because the critical section won't work.
     * @param nitd Network interval data
     */

    void IpPacketReassemblerFeature::expireOldEntries(NetworkIntervalTickData& nitd) {

      // ensure any higher priority IRQs can't come along and pre-empt us

      IrqSuspend suspender;

      // check each entry and delete if expired

      for(auto it=_frags.begin();it!=_frags.end();) {

        if(nitd.timeNow>(*it)->expiryTime) {

          _totalFragmentSize-=(*it)->packetLength;
          _inFlightPacketCount--;

          delete *it;
          it=_frags.erase(it);
        }
        else
          it++;
      }
    }


    /**
     * Free a packet and take it out of the linked list and update
     * all statuses
     * @param fp The packet to free
     */

    void IpPacketReassemblerFeature::ip_freePacket(IpFragmentedPacket *packetToFree) {

      // ensure we cannot be interrupted

      IrqSuspend suspender;

      // find and free the packet

      for(auto it=_frags.begin();it!=_frags.end();it++) {
        if(*it==packetToFree) {
          _frags.erase(it);
          break;
        }
      }

      // free the data, update counts

      _totalFragmentSize-=packetToFree->packetLength;
      _inFlightPacketCount--;

      // free packet structure

      delete packetToFree;
    }


    /**
     * Find a fragmented packet in the list of in-progress fragments
     * @param pid the id of the packet to find
     * @return the fragments so far or nullptr
     */

    IpFragmentedPacket *IpPacketReassemblerFeature::findFragment(const IpFragmentedPacket::PacketId& pid) const {

      for(auto it=_frags.begin();it!=_frags.end();it++)
        if((*it)->identifier==pid)
          return *it;

      return nullptr;
    }


    /**
     * Create a new fragment, add to the list and return the FragmentedPacket structure
     * @param pid The packet id
     * @param[out] fp The new FragmentedPacket structure
     * @return true if it works, false if a limit is exceeded
     */

    bool IpPacketReassemblerFeature::createNewFragment(IpFragmentedPacket::PacketId pid,IpFragmentedPacket *&fp) {

      // check for the maximum fragmented packets

      if(_frags.size()==_params.ip_maxInProgressFragmentedPackets)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER,E_TOO_MANY_FRAGMENTED_PACKETS);

      // create the new entry (expiry time is not set here)

      fp=new IpFragmentedPacket(pid);

      // link it in at the front

      _frags.push_front(fp);

      // update the inflight count and memory usage

      _inFlightPacketCount++;
      _totalFragmentSize+=fp->packetLength;

      return true;
    }
  }
}


#endif
