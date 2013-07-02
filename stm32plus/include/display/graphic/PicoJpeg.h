//------------------------------------------------------------------------------
// picojpeg v1.0 - Public domain, Rich Geldreich <richgel99@gmail.com>
//------------------------------------------------------------------------------

#pragma once


namespace stm32plus {
  namespace display {

    // Error codes
    enum {
      PJPG_NO_MORE_BLOCKS=1,
      PJPG_BAD_DHT_COUNTS,
      PJPG_BAD_DHT_INDEX,
      PJPG_BAD_DHT_MARKER,
      PJPG_BAD_DQT_MARKER,
      PJPG_BAD_DQT_TABLE,
      PJPG_BAD_PRECISION,
      PJPG_BAD_HEIGHT,
      PJPG_BAD_WIDTH,
      PJPG_TOO_MANY_COMPONENTS,
      PJPG_BAD_SOF_LENGTH,
      PJPG_BAD_VARIABLE_MARKER,
      PJPG_BAD_DRI_LENGTH,
      PJPG_BAD_SOS_LENGTH,
      PJPG_BAD_SOS_COMP_ID,
      PJPG_W_EXTRA_BYTES_BEFORE_MARKER,
      PJPG_NO_ARITHMITIC_SUPPORT,
      PJPG_UNEXPECTED_MARKER,
      PJPG_NOT_JPEG,
      PJPG_UNSUPPORTED_MARKER,
      PJPG_BAD_DQT_LENGTH,
      PJPG_TOO_MANY_BLOCKS,
      PJPG_UNDEFINED_QUANT_TABLE,
      PJPG_UNDEFINED_HUFF_TABLE,
      PJPG_NOT_SINGLE_SCAN,
      PJPG_UNSUPPORTED_COLORSPACE,
      PJPG_UNSUPPORTED_SAMP_FACTORS,
      PJPG_DECODE_ERROR,
      PJPG_BAD_RESTART_MARKER,
      PJPG_ASSERTION_ERROR,
      PJPG_BAD_SOS_SPECTRAL,
      PJPG_BAD_SOS_SUCCESSIVE,
      PJPG_STREAM_READ_ERROR,
      PJPG_NOTENOUGHMEM,
      PJPG_UNSUPPORTED_COMP_IDENT,
      PJPG_UNSUPPORTED_QUANT_TABLE,
      PJPG_UNSUPPORTED_MODE
    };

  // Scan types - currently only GRAYSCALE, YH1V1, and YH2V2 are actually supported.
    typedef enum {
      PJPG_GRAYSCALE, PJPG_YH1V1, PJPG_YH2V1, PJPG_YH1V2, PJPG_YH2V2
    } pjpeg_scan_type_t;

  #define MAX_IN_BUF_SIZE 256

    typedef struct HuffTableT {
        uint16_t mMinCode[16];
        uint16_t mMaxCode[16];
        uint8_t mValPtr[16];
    } HuffTable;

    typedef struct {
        // Image resolution
        int m_width;
        int m_height;
        // Number of components (1 or 3)
        int m_comps;
        // Total number of minimum coded units (MCU's) per row/col.
        int m_MCUSPerRow;
        int m_MCUSPerCol;
        // Scan type
        pjpeg_scan_type_t m_scanType;
        // MCU width/height in pixels
        int m_MCUWidth;
        int m_MCUHeight;
        // Pointers to internal MCU pixel component buffers.
        // These buffers Will be filled with pixels each time pjpegDecodeMCU() is called successfully.
        // Each MCU consists of (m_MCUWidth/8)*(m_MCUHeight/8) blocks (currently either 1 for greyscale/no subsampling, or 4 for H2V2 sampling factors), where each block is a contiguous array of 64 (8x8) bytes.
        // For greyscale images, only the values in m_pMCUBufR are valid.
        unsigned char *m_pMCUBufR;
        unsigned char *m_pMCUBufG;
        unsigned char *m_pMCUBufB;

        // Andy note: The following are moved from being a hardcoded hole in .data to dynamic
        // allocation on the stack.

        int16_t gCoeffBuf[8 * 8];

        // 8*8*4 bytes * 3 = 768
        uint8_t gMCUBufR[256];
        uint8_t gMCUBufG[256];
        uint8_t gMCUBufB[256];

        // 256 bytes
        int16_t gQuant0[8 * 8];
        int16_t gQuant1[8 * 8];

        uint8_t gInBuf[MAX_IN_BUF_SIZE];

        // DC - 192
        HuffTable gHuffTab0;

        uint8_t gHuffVal0[16];

        HuffTable gHuffTab1;
        uint8_t gHuffVal1[16];

        // AC - 672
        HuffTable gHuffTab2;
        uint8_t gHuffVal2[256];

        HuffTable gHuffTab3;
        uint8_t gHuffVal3[256];
    } pjpeg_image_info_t;


  // Initializes the decompressor. Returns 0 on success, or one of the above error codes on failure.
  // pNeed_bytes_callback will be called to fill the decompressor's internal input buffer.
  // Not thread safe.

    uint8_t pjpeg_decode_init(pjpeg_image_info_t *pInfo,InputStream& is);

  // Decompresses the file's next MCU. Returns 0 on success, PJPG_NO_MORE_BLOCKS if no more blocks are available, or an error code.
  // Must be called a total of m_MCUSPerRow*m_MCUSPerCol times to completely decompress the image.
    uint8_t pjpeg_decode_mcu(void);
  }
}
