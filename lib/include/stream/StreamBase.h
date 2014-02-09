/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Abstract class defining how an input stream behaves.
   *
   * Input streams give you the ability to read data sequentially. The base
   * class supplies an implementation of the popular stream operators
   * << and >>. These should be used with care as error handling is
   * not easily implemented.
   */

  class StreamBase {

    protected:
      virtual ~StreamBase() {}

    public:

      /**
       * Error codes
       */

      enum {
        /// End of stream has been reached
        E_END_OF_STREAM=-1,

        /// An error occurred while reading from the stream
        E_STREAM_ERROR=-2,

        /// The operation is not supported
        E_OPERATION_NOT_SUPPORTED=-3
      };

    public:

      /**
       * Close the stream. If the stream does not support the operation
       * then it returns true.
       * @return false if it fails.
       */

      virtual bool close()=0;
  };
}
