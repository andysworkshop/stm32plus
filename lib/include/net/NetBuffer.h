/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Network buffer class. This is designed to hold an un-owned pointer to an optional
     * user-supplied data buffer intended for transmission together with an owned buffer
     * large enough to contain data to transmit before the user buffer, i.e. network
     * headers and such like.
     */

    class NetBuffer {

      protected:
        const void *_userBuffer;
        uint32_t _userBufferSize;

        void *_internalBuffer;
        uint32_t _internalBufferSize;

        void *_writePointer;

        // this is normall nullptr except when a fragmented packet is going out. In that case the last
        // in the sequence has the original packet pointer here

        scoped_ptr<NetBuffer> _reference;

        // this is inserted by the network layer so the datalink layer knows whether it has to
        // compute a checksum or not

        DatalinkChecksum _checksumRequest;

      public:
        NetBuffer(uint32_t headerSpace,uint32_t dataSpace,const void *userBuffer=nullptr,uint32_t userBufferSize=0);
        ~NetBuffer();

        const void *getUserBuffer() const;
        void *getInternalBuffer() const;

        uint32_t getUserBufferSize() const;
        uint32_t getInternalBufferSize() const;
        uint32_t getSizeFromWritePointerToEnd() const;

        void *getWritePointer() const;
        void *moveWritePointerBack(uint32_t size);

        NetBuffer *getReference() const;
        void setReference(NetBuffer *reference);

        DatalinkChecksum getChecksumRequest() const;
        void setChecksumRequest(DatalinkChecksum checksumRequest);
    };


    /**
     * Constructor, save variables and allocate internal buffer
     * @param headerSpace
     * @param dataSpace
     * @param userBuffer
     */

    inline NetBuffer::NetBuffer(uint32_t headerSpace,uint32_t dataSpace,const void *userBuffer,uint32_t userBufferSize) {

      _userBufferSize=userBufferSize;
      _userBuffer=userBuffer;

      // allocate space for net buffer and position the write pointer past the end

      _internalBufferSize=headerSpace+dataSpace;
      _internalBuffer=malloc(_internalBufferSize);
      _writePointer=reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(_internalBuffer)+_internalBufferSize);
    }


    /**
     * Destructor, clean up
     */

    inline NetBuffer::~NetBuffer() {
      if(_internalBuffer)
        free(_internalBuffer);
    }


    /**
     * Get the size of the user buffer
     * @return The user buffer size
     */

    inline uint32_t NetBuffer::getUserBufferSize() const {
      return _userBufferSize;
    }


    /**
     * Return the user buffer pointer
     */

    inline const void *NetBuffer::getUserBuffer() const {
      return _userBuffer;
    }


    /**
     * Return the internal buffer pointer
     */

    inline void *NetBuffer::getInternalBuffer() const {
      return _internalBuffer;
    }


    /**
     * Get the internal buffer size
     * @return The internal buffer size
     */

    inline uint32_t NetBuffer::getInternalBufferSize() const {
      return _internalBufferSize;
    }


    /**
     * Get the size of the buffer from the current write pointer
     * to the end of the data. This is effectively a "total packet
     * size" when called for a protocol whose header begins at the
     * current write pointer
     * @return The size requested
     */

    inline uint32_t NetBuffer::getSizeFromWritePointerToEnd() const {
      return (reinterpret_cast<uint32_t>(_internalBuffer)+_internalBufferSize)-
             reinterpret_cast<uint32_t>(_writePointer);
    }


    /**
     * Get the write pointer
     * @return the current write pointer
     */

    inline void *NetBuffer::getWritePointer() const {
      return _writePointer;
    }


    /**
     * Decrement the write pointer by the given size and return its
     * new position
     * @return the new position
     */

    inline void *NetBuffer::moveWritePointerBack(uint32_t size) {
      _writePointer=reinterpret_cast<uint8_t *>(_writePointer)-size;
      return _writePointer;
    }


    /**
     * Get the 'reference' netbuffer. This will be non-null if this netbuffer
     * is the last in a sequence of fragmented packets. Basically the reference pointer
     * serves to keep the original bumper packet in scope while the fragments are transmitted. This
     * means that the fragments can exist as pointers into the original buffer without having to
     * create new memory buffers with all the memory and CPU overhead that would entail.
     * @return The pointer
     */

    inline NetBuffer *NetBuffer::getReference() const {
      return _reference.get();
    }


    /**
     * Set a new reference pointer. Can only be called once.
     * @param reference The new reference pointer
     */

    inline void NetBuffer::setReference(NetBuffer *reference) {
      _reference.reset(reference);
    }


    /**
     * Get the checksum request type
     * @return The checksum request type
     */

    inline DatalinkChecksum NetBuffer::getChecksumRequest() const {
      return _checksumRequest;
    }


    /**
     * Set the checksum request flag
     * @param checksumRequest The flag
     */

    inline void NetBuffer::setChecksumRequest(DatalinkChecksum checksumRequest) {
      _checksumRequest=checksumRequest;
    }
  }
}
