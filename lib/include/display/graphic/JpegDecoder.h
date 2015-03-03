/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * JPEG decoder. Implements the callback from the picoJpeg decoder to write
     * 8x8 blocks to the screen.
     *
     * Either call decode() to decode the whole JPEG or call beginDecode() then
     * endDecode() if you need access to the image dimensions
     */

    template<class TGraphicsLibrary>
    class JpegDecoder {

      protected:
        pjpeg_image_info_t _imageInfo;

      public:

        /**
         * Convenience method to call begin, end
         * @param pt
         * @param is
         * @param gl
         */

        bool decode(const Point& pt,InputStream& is,TGraphicsLibrary& gl) {

          Size size;

          if(!beginDecode(is,size))
            return false;

          return endDecode(pt,gl);
        }


        /**
         * Start decoding.
         * @param is
         * @param size
         * @return true if it works
         */

        bool beginDecode(InputStream& is,Size& size) {

          // initialise the decoder

          if(pjpeg_decode_init(&_imageInfo,is)!=0)
            return false;

          size.Width=_imageInfo.m_width;
          size.Height=_imageInfo.m_height;

          return true;
        }


        /**
         * Decode the JPEG encoded data from the input stream, using the graphics library and display it
         * at the point on screen.
         * @param pt
         * @param gl
         */

        bool endDecode(const Point& pt,TGraphicsLibrary& gl) {

          typename TGraphicsLibrary::UnpackedColour cr;

          int16_t mcu_x=0,mcu_y=0;

          for(;;) {

            if(pjpeg_decode_mcu()!=0)
              return false;

            if(mcu_y>=_imageInfo.m_MCUSPerCol)
              return false;

            for(int y=0;y<_imageInfo.m_MCUHeight;y+=8) {

              int by_limit=std::min<int>(8,_imageInfo.m_height-(mcu_y*_imageInfo.m_MCUHeight+y));

              for(int x=0;x<_imageInfo.m_MCUWidth;x+=8) {

                uint16_t src_ofs=(x*8U)+(y*16U);

                uint8_t *pSrcR=_imageInfo.m_pMCUBufR+src_ofs;
                uint8_t *pSrcG=_imageInfo.m_pMCUBufG+src_ofs;
                uint8_t *pSrcB=_imageInfo.m_pMCUBufB+src_ofs;

                int bx_limit=std::min(8,_imageInfo.m_width-(mcu_x*_imageInfo.m_MCUWidth+x));

                gl.moveTo(Rectangle(pt.X+mcu_x*_imageInfo.m_MCUWidth+x,pt.Y+mcu_y*_imageInfo.m_MCUHeight+y,bx_limit,by_limit));
                gl.beginWriting();

                if(_imageInfo.m_scanType==PJPG_GRAYSCALE) {

                  for(int by=0;by<by_limit;by++) {

                    for(int bx=0;bx < bx_limit;bx++) {
                      gl.unpackColour(*pSrcR,*pSrcR,*pSrcR,cr);
                      gl.writePixel(cr);
                      pSrcR++;
                    }

                    pSrcR+=(8-bx_limit);
                  }
                } else {
                  for(int by=0;by<by_limit;by++) {

                    for(int bx=0;bx<bx_limit;bx++) {

                      gl.unpackColour(*pSrcR,*pSrcG,*pSrcB,cr);

                      pSrcR++;
                      pSrcG++;
                      pSrcB++;

                      gl.writePixel(cr);
                    }

                    pSrcR+=(8-bx_limit);
                    pSrcG+=(8-bx_limit);
                    pSrcB+=(8-bx_limit);
                  }
                }
              }
            }

            mcu_x++;

            if(mcu_x==_imageInfo.m_MCUSPerRow) {
              mcu_x=0;
              mcu_y++;
            }
          }

          return true;
        }
    };
  }
}

