/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Linked list entry for a fragmented packet
     */

    struct IpFragmentedPacket {

      /**
       * Identifying information for a packet
       */

      struct PacketId {

        uint16_t identification;
        IpAddress sourceAddress;
        IpAddress destinationAddress;
        IpProtocol protocol;

        bool operator==(const PacketId& src) const {
          return identification==src.identification &&
                 destinationAddress==src.destinationAddress &&
                 sourceAddress==src.sourceAddress &&
                 protocol==src.protocol;
        }

        bool operator!=(const PacketId& src) const {
          return !operator==(src);
        }
      } __attribute__((packed));


      /**
       * The hole structure identifies the size and position of the hole and
       * the offset from the beginning of the packet buffer of the next and
       * previous hole. UINT16_T_MAX == no previous or next. This is safe because
       * the data-block size of 8 bytes means that nothing can start at 0xffff.
       *
       * This structure MUST NOT exceed 8 bytes.
       */

      struct Hole {
        uint16_t first;
        uint16_t last;
        uint16_t next_hole;

        static Hole *getPointer(uint8_t *base,uint16_t offset) {
          return offset==UINT16_MAX ? nullptr : reinterpret_cast<Hole *>(&base[offset]);
        }

        static uint16_t getOffset(uint8_t *base,Hole *hole) {
          return reinterpret_cast<uint8_t *>(hole)-base;
        }

      } __attribute__((packed));

      PacketId identifier;

      uint16_t packetLength;
      uint8_t *packet;
      uint16_t firstHole;
      uint32_t expiryTime;

      IpFragmentedPacket(PacketId pid);
      ~IpFragmentedPacket();

      void handleHoleList(const IpPacket& packet);
      void createHole(uint16_t first,uint16_t last);
      void unlinkHole(uint16_t holeToUnlink);
      bool isComplete() const;
    };


    /**
     * Fragmented packet: constructor
     * Set the identifier and set up the first hole in a buffer 8 bytes long
     */

    inline IpFragmentedPacket::IpFragmentedPacket(PacketId pid) {

      Hole *ptr;

      identifier=pid;
      packetLength=8;
      packet=reinterpret_cast<uint8_t *>(malloc(8));

      firstHole=0;
      ptr=Hole::getPointer(packet,0);

      ptr->first=0;
      ptr->last=UINT16_MAX;
      ptr->next_hole=UINT16_MAX;
    }

    /**
     * FragmentedPacket: destructor
     */

    inline IpFragmentedPacket::~IpFragmentedPacket() {
      free(packet);
    }


    /**
     * Create a new hole and link it in. The hole actually lives in the packet data space
     * starting at the address of 'first'
     * @param first
     * @param last
     */

    inline void IpFragmentedPacket::createHole(uint16_t first,uint16_t last) {

      Hole *hole;

      // create the hole descriptor and link it in at the start

      hole=reinterpret_cast<Hole *>(&packet[first]);
      hole->first=first;
      hole->last=last;
      hole->next_hole=firstHole;

      firstHole=first;
    }


    /**
     * Unlink this hole from the list
     * @param holeToUnlink
     */

    inline void IpFragmentedPacket::unlinkHole(uint16_t holeToUnlink) {

      Hole *holeptr,*previousptr,*firstptr,*unlinkptr;

      firstptr=Hole::getPointer(packet,firstHole);
      unlinkptr=Hole::getPointer(packet,holeToUnlink);

      if(firstHole==holeToUnlink) {
        firstHole=firstptr->next_hole;
        return;
      }

      previousptr=firstptr;
      holeptr=Hole::getPointer(packet,firstptr->next_hole);

      while(holeptr) {

        if(holeptr==unlinkptr) {

          previousptr->next_hole=holeptr->next_hole;
          return;
        }

        previousptr=holeptr;
        holeptr=Hole::getPointer(packet,holeptr->next_hole);
      }
    }


    /**
     * Check if the reassembly is complete
     * @return true if it's complete
     */

    inline bool IpFragmentedPacket::isComplete() const {
      return firstHole==UINT16_MAX;
    }


    /**
     * Updates holes to handle this packet
     * @param fp The fragmented packet that we are filling in
     * @param packet The new packet fragment
     */

    inline void IpFragmentedPacket::handleHoleList(const IpPacket& ipPacket) {

      Hole *hole;
      uint16_t fragmentFirst,fragmentLast,holeFirst,holeLast,holeNext;

      fragmentFirst=ipPacket.getFragmentOffset();
      fragmentLast=fragmentFirst+ipPacket.payloadLength-1;

      hole=nullptr;

      for(;;) {

        // advance to next (or first): RFC815 #1

        if(hole)
          hole=Hole::getPointer(packet,holeNext);
        else
          hole=Hole::getPointer(packet,firstHole);

        if(hole==nullptr)
          return;

        holeFirst=hole->first;
        holeLast=hole->last;
        holeNext=hole->next_hole;

        // RFC815 steps #2,3

        if(fragmentFirst>holeLast || fragmentLast<holeFirst)
          continue;

        // delete this entry from the descriptor list. RFC815 step #4

        unlinkHole(Hole::getOffset(packet,hole));

        // RFC815 step #5

        if(fragmentFirst>holeFirst)
          createHole(holeFirst,fragmentFirst-1);

        // RFC815 step #6

        if(fragmentLast<holeLast && ipPacket.hasMoreFragments())
          createHole(fragmentLast+1,holeLast);
      }
    }
  }
}
