/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Draw a bitmap on to the display. The size and position of the bitmap on the display must match the source. There is
     * no stretch or shrink operation here. The CPU is used to copy data.
     *
     * @param rect The size and position of the rectangle on the display.
     * @param source The input stream supplying the source data
     * @return false if there was a failure (e.g. stream failure)
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline bool GraphicsLibrary<TDevice,TDeviceAccessMode>::drawBitmap(const Rectangle& rect,InputStream& source) {

      int16_t vpos;
      uint8_t *buffer;
      uint32_t actuallyRead,bytesPerPixel;
      bool retval;

      // move to the rect defined by the bitmap

      this->moveTo(rect);

      // allocate space for even scan lines and odd scan lines

      this->allocatePixelBuffer(rect.Width,buffer,bytesPerPixel);

      retval=false;

      // ready to start writing to the display

      this->beginWriting();

      // write it scan-by-scan. there's not likely to be enough memory to
      // do it all in one big read from the stream

      for(vpos=0;vpos<rect.Height;vpos++) {

        // read a line

        if(!source.read(buffer,rect.Width*bytesPerPixel,actuallyRead) || actuallyRead!=bytesPerPixel*rect.Width)
          goto finished;

        // draw it

        this->rawTransfer(buffer,rect.Width);
      }

      // succeeded

      retval=true;

      finished:

      // clean up memory

      delete[] buffer;

      return retval;
    }


    /**
     * Draw a bitmap on to the display. The size and position of the bitmap on the display must match the source. There is
     * no stretch or shrink operation here. The DMA channel is used to transfer data to the FSMC. That implies that the
     * access mode being used is the FSMC. Compilation will fail for other access modes.
     *
     * @param rect The size and position of the rectangle on the display.
     * @param source The input stream supplying the source data
     * @param dma The DMA class used to transfer the data. Note that there may be limits (e.g. on the F4) on which DMA peripherals can perform memory to memory transfers.
     * @param priority The dma priority constant
     * @return false if there was a failure (e.g. stream failure)
     */

    template<class TDevice,typename TDeviceAccessMode>
    template<class TDmaCopierImpl>
    inline bool GraphicsLibrary<TDevice,TDeviceAccessMode>::drawBitmap(const Rectangle& rect,
                                                                       InputStream& source,
                                                                       DmaLcdWriter<TDmaCopierImpl>& dma,
                                                                       uint32_t priority) {

      int16_t vpos;
      uint8_t *evenLines,*oddLines,*buffer;
      uint32_t actuallyRead,bytesPerPixel;
      bool retval;

      // move to the rect defined by the bitmap

      this->moveTo(rect);

      // allocate space for even scan lines and odd scan lines

      this->allocatePixelBuffer(rect.Width,evenLines,bytesPerPixel);
      this->allocatePixelBuffer(rect.Width,oddLines,bytesPerPixel);

      // ready to start writing to the display

      this->beginWriting();

      // write it scan-by-scan. there's not likely to be enough memory to
      // do it all in one big read from the stream

      retval=false;

      for(vpos=0;vpos<rect.Height;vpos++) {

        // point to the appropriate buffer

        buffer=(vpos & 1)==0 ? evenLines : oddLines;

        // read a line

        if(!source.read(buffer,rect.Width*bytesPerPixel,actuallyRead) || actuallyRead!=bytesPerPixel*rect.Width)
          goto finished;

        // wait for the last line to complete

        if(vpos>0 && !dma.waitUntilComplete())
          goto finished;

        // transfer the scan line of bytes

        dma.beginCopyToLcd((void *)this->_accessMode.getDataAddress(),buffer,rect.Width*bytesPerPixel,priority);
      }

      // succeeded

      retval=true;

      finished:

      // clean up memory

      delete[] evenLines;
      delete[] oddLines;

      // wait for the last scan line to transfer

      if(retval)
        retval=dma.waitUntilComplete();

      return retval;
    }


    /**
     * Draw a JPEG on the display. The rectangle size must match the JPEG size. The source
     * should supply the compressed data in the form of a JPEG file. Progressive JPEGs are
     * not supported. This function will cost you about 2Kb of SRAM to call.
     *
     * @param rc The rectangle to draw the image at.
     * @param source The source of compressed data.
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::drawJpeg(const Rectangle& rc,InputStream& source) {

      // call a decoder typed for this graphics library

      JpegDecoder<GraphicsLibrary<TDevice,TDeviceAccessMode>> jpeg;
      jpeg.decode(rc.getTopLeft(),source,*this);
    }
  }
}
