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
     * Create an IPv4 fragment collection from the input buffer.
     * @param inputBuffer[in] The input buffer to fragment
     * @param outputBuffers[out] The collection of output buffers. This should be free'd
     *   using free() when the buffers have been accepted by the link layer. Do NOT
     *   use operator delete[] because that would destroy the objects whose ownership
     *   has been transferred to the link layer.
     * @param outputBufferCount[out] The number of net buffers in the output
     * @param mtu The max MTU size for the link layer (excluding its own headers)
     * @param linkHeaderSize The size of the headers for the link layer
     * @param protocol The IP protocol in this packet
     * @param myIpAddress this IP address
     * @param destinationIpAddress the target address
     * @return true if it worked. false if it failed.
     */

    bool IpPacketFragmentFeature::ip_createFragments(
                                    IpProtocol protocol,
                                    const IpAddress& myIpAddress,
                                    const IpAddress& destinationIpAddress,
                                    uint16_t mtu,
                                    uint16_t linkHeaderSize,
                                    NetBuffer *inputBuffer,
                                    NetBuffer **&outputBuffers,
                                    uint16_t& outputBufferCount) {

      uint16_t i;

      _mtu=mtu;
      _linkHeaderSize=linkHeaderSize;

      // MTU - IP_HEADER_SIZE must be a multiple of 8 bytes. reduce it to the
      // nearest multiple if it's not

      _mtu=((_mtu-IpPacketHeader::getNoOptionsHeaderSize()) & ~7)+IpPacketHeader::getNoOptionsHeaderSize();

      // start with a new ID (first = 1)

      _identification++;

      // The STM32 MAC cannot do checksum offload for fragmented packets so we offer the option
      // to do it in software for UDP packets.

      if(_params.ip_checksumOnLargeUdpPackets && protocol==IpProtocol::UDP)
        InternetChecksum::calculate(myIpAddress,destinationIpAddress,*inputBuffer);

      // work out how many fragments we need. we fragment the internal buffer and the
      // user buffer separately resulting in one more fragment than is strictly
      // necessary in some cases

      outputBufferCount=fragmentsRequired(inputBuffer->getSizeFromWritePointerToEnd())
                       +fragmentsRequired(inputBuffer->getUserBufferSize());

      // allocate for the output buffers

      outputBuffers=reinterpret_cast<NetBuffer **>(malloc(sizeof(NetBuffer)*outputBufferCount));
      if(outputBuffers==nullptr)
        return false;

      memset(outputBuffers,0,sizeof(NetBuffer *)*outputBufferCount);

      // attempt to do the fragmentation

      if(internalCreateFragments(inputBuffer,outputBuffers))
        return true;

      // failed, clean up any memory allocated and return failure

      for(i=0;i<outputBufferCount;i++)
        if(outputBuffers[i])
          delete outputBuffers[i];

      free(outputBuffers);
      outputBuffers=nullptr;

      return false;
    }


    /*
     * Create fragments, internal function. buffers and count are set up.
     */

    bool IpPacketFragmentFeature::internalCreateFragments(NetBuffer *inputBuffer,NetBuffer **&outputBuffers) {

      uint16_t offset;

      // first fragment the internal buffer - there will at least
      // be a header in here from the upper layers

      offset=0;

      if(!createFragmentsFrom(inputBuffer,
                              inputBuffer->getWritePointer(),
                              inputBuffer->getSizeFromWritePointerToEnd(),
                              outputBuffers,
                              0,
                              inputBuffer->getUserBufferSize()==0,
                              offset))
        return false;

      // if there's no user data then that's it

      if(inputBuffer->getUserBufferSize()==0)
        return true;

      // finally fragment the user data

      return createFragmentsFrom(inputBuffer,
                                 inputBuffer->getUserBuffer(),
                                 inputBuffer->getUserBufferSize(),
                                 outputBuffers,
                                 fragmentsRequired(inputBuffer->getSizeFromWritePointerToEnd()),
                                 true,
                                 offset);
    }


    /**
     * Create a run of fragmented packets from this data
     * @param inputBuffer The netbuffer containing the data to fragment
     * @param sourceData The source data to fragment
     * @param sourceDataSize The size of sourceData in bytes
     * @param isInternalBufferData true if this data is coming from the internal buffer
     * @param outputBuffers Where to store the netbuffers containing the fragments
     * @param firstOutputBuffer The index of the first netbuffer to store the fragments in
     * @param lastFragmentIsHere true if the last fragment in this packet is in this data
     * @return true if it worked. false means out of memory.
     */

    bool IpPacketFragmentFeature::createFragmentsFrom(NetBuffer *inputBuffer,
                                                       const void *sourceData,
                                                       uint16_t sourceDataSize,
                                                       NetBuffer **outputBuffers,
                                                       uint16_t firstOutputBuffer,
                                                       bool lastFragmentIsHere,
                                                       uint16_t& offset) {

      NetBuffer *nb;
      uint16_t fragmentSize,flags;
      const uint8_t *ptr;

      ptr=reinterpret_cast<const uint8_t *>(sourceData);

      while(sourceDataSize) {

        // calculate sizes

        fragmentSize=sourceDataSize<_mtu-IpPacketHeader::getNoOptionsHeaderSize() ?
                      sourceDataSize :
                      _mtu-IpPacketHeader::getNoOptionsHeaderSize();

        // create a new netbuffer for the fragment

        nb=new NetBuffer(_linkHeaderSize+IpPacketHeader::getNoOptionsHeaderSize(),  // needs IP and link layer headers
                         0,
                         ptr,                                 // fragment data starts here
                         fragmentSize);                       // size of this fragment

        // more fragments flag if this is not the last in the run

        if(lastFragmentIsHere && fragmentSize==sourceDataSize) {

          // this is the last

          flags=0;
          nb->setReference(inputBuffer);
        }
        else
          flags=MORE_FRAGMENTS;

        // insert the IP fragment header

        IpPacketHeader *header=reinterpret_cast<IpPacketHeader *>(nb->moveWritePointerBack(IpPacketHeader::getNoOptionsHeaderSize()));

        // set up only the parts that are unique to the fragmenter

        header->ip_hdr_length=NetUtil::htons(IpPacketHeader::getNoOptionsHeaderSize()+fragmentSize);
        header->ip_hdr_identification=_identification;
        header->ip_hdr_flagsAndOffset=NetUtil::htons((offset/8) | flags);

        // store the netbuffer in the array

        outputBuffers[firstOutputBuffer++]=nb;

        // update pointers

        ptr+=fragmentSize;
        sourceDataSize-=fragmentSize;
        offset+=fragmentSize;
      }

      return true;
    }


    /**
     * Calculate the number of fragments required to send 'size' bytes with an
     * @param mtu The max MTU size of the link layer
     * @param size The number of bytes to send
     * @return The number of fragments required
     */

    uint32_t IpPacketFragmentFeature::fragmentsRequired(uint16_t size) {
      return size/(_mtu-IpPacketHeader::getNoOptionsHeaderSize())+(size % (_mtu-IpPacketHeader::getNoOptionsHeaderSize()) ? 1 : 0);
    }
  }
}


#endif
