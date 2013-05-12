/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace display {

		/**
		 * JPEG decoder. Implements the callback from the picoJpeg decoder to write
		 * 8x8 blocks to the screen.
		 */

		template<class TGraphicsLibrary>
		class JpegDecoder {

			//typedef typename TGraphicsLibrary::template UnpackedColour<TColourDepth> UNPACKED_COLOUR;

			public:

				/**
				 * Decode the JPEG encoded data from the input stream, using the graphics library and display it
				 * at the point on screen.
				 * @param pt
				 * @param is
				 * @param gl
				 */

				static void decode(const Point& pt,InputStream& is,TGraphicsLibrary& gl) {

					pjpeg_image_info_t imageInfo;
					typename TGraphicsLibrary::UnpackedColour cr;

					// initialise the decoder

					if(pjpeg_decode_init(&imageInfo,is)!=0)
						return;

					int16_t mcu_x=0,mcu_y=0;

					for(;;) {

						if(pjpeg_decode_mcu()!=0)
							return;

						if(mcu_y>=imageInfo.m_MCUSPerCol)
							return;

						for(int y=0;y<imageInfo.m_MCUHeight;y+=8) {

							int by_limit=Min<int>(8,imageInfo.m_height-(mcu_y*imageInfo.m_MCUHeight+y));

							for(int x=0;x<imageInfo.m_MCUWidth;x+=8) {

								uint16_t src_ofs=(x*8U)+(y*16U);

								uint8_t *pSrcR=imageInfo.m_pMCUBufR+src_ofs;
								uint8_t *pSrcG=imageInfo.m_pMCUBufG+src_ofs;
								uint8_t *pSrcB=imageInfo.m_pMCUBufB+src_ofs;

								int bx_limit=Min(8,imageInfo.m_width-(mcu_x*imageInfo.m_MCUWidth+x));

								gl.moveTo(Rectangle(pt.X+mcu_x*imageInfo.m_MCUWidth,pt.Y+mcu_y*imageInfo.m_MCUHeight,bx_limit,by_limit));
								gl.beginWriting();

								if(imageInfo.m_scanType==PJPG_GRAYSCALE) {

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

						if(mcu_x==imageInfo.m_MCUSPerRow) {
							mcu_x=0;
							mcu_y++;
						}
					}
				}
		};
	}
}

