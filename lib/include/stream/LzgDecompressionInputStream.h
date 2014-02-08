/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * @brief Decompress LZG-compressed bytes
   *
   * LzgDecompressionInputStream acts as a filter, taking LZG-compressed bytes from an
   * input stream that you supply and making them available as an uncompressed stream
   * through this class's own implementation of InputStream.
   */

  class LzgDecompressionStream : public InputStream {

    public:
      enum {
        E_UNSUPPORTED_COMPRESSED_DATA = 1
      };

    protected:
      InputStream& _input;
      uint32_t _compressedSize;

      uint32_t _compressedDataAvailable;
      uint8_t _circbuf[2056];               // note the size of this - ensure you can afford it
      uint8_t *_dst,*_dstEnd;
      char _isMarkerSymbolLUT[256];
      uint8_t _marker1,_marker2,_marker3,_marker4;

      uint8_t *_historyCopyPosition;
      uint32_t _historyCopyDataAvailable;

    protected:
      bool readNextUncompressedByte(uint8_t& nextByte);
      bool nextByteFromStream(uint8_t& nextByte);
      uint8_t getByteFromHistoryCopy();

    public:
      LzgDecompressionStream(InputStream& input,uint32_t compressedSize);
      virtual ~LzgDecompressionStream() {}

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool close() override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;
  };
}
