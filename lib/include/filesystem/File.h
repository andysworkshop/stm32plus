/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for file objects
   */

  class File {

    protected:
      uint32_t _offset;

    public:

      /**
       * The valid seek start positions.
       */

      enum SeekFrom {
        /// Start from the beginning of the file.
        SeekStart,

        /// Start from the end of the file.
        SeekEnd,

        /// Start from the current file position
        SeekCurrent
      };

      /**
       * Error codes
       */

      enum {
        /// A requested seek position is not valid
        E_INVALID_FILE_POSITION=1,

        /// The end of the file has been reached
        E_END_OF_FILE,

        /// The file is unusable because it contains a bad cluster
        E_BAD_CLUSTER
      };

    public:
      File();

    /**
     * Virtual destructor, does nothing
     */
      virtual ~File() {
      }


      virtual uint32_t getOffset() const;

    /**
     * Read from the file. If the end of file is hit then actuallyRead_ will be shorter
     * than the amount requested.
     *
     * @param[in,out] ptr_ Caller supplied buffer to write to.
     * @param[in] size_ The number of bytes to attempt to read.
     * @param[out] actuallyRead_ The number of bytes actually read.
     * @return false if it fails, or end of file is hit. Check error provider for E_END_OF_FILE for this case.
     */

      virtual bool read(void *ptr_,uint32_t size_,uint32_t& actuallyRead_)=0;

    /**
     * Write to the file.
     *
     * @param[in] ptr_ The buffer to write.
     * @param[in] size_ The number of bytes to write.
     * @return false if it fails.
     */

      virtual bool write(const void *ptr_,uint32_t size_)=0;

    /**
     * Seek to a given offset within the file. Valid locations to seek to are from zero to the file
     * length. Seeking to the file length moves the pointer to one past the end of the file data so
     * that subsequent file writes append to the existing file.

     * @param[in] offset_ The distance to move from the origin_ parameter.
     * @param[in] origin_ One of the SeekFrom enumeration.
     */

      virtual bool seek(int32_t offset_,SeekFrom origin_)=0;

    /**
     * Get the file length.
     * @return The file length up to the maximum supported 4Gb.
     */

      virtual uint32_t getLength()=0;
  };
}
