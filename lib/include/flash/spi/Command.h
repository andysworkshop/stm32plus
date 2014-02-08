/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Base template for SPI commands. Provides the actual functionality to execute
     * the command
     */

    template<class TSpi>
    class Command : public virtual Base<TSpi> {

      protected:
        bool writeCommand(uint8_t command) const;
        bool writeCommand(uint8_t command,uint32_t address,uint8_t addressBytes,uint8_t dummyBytes,const void *data,uint32_t dataSize) const;
        bool readCommand(uint8_t command,uint32_t address,uint8_t addressBytes,uint8_t dummyBytes,void *data,uint32_t dataSize) const;

        bool doWriteCommand(uint8_t command,uint32_t address,uint8_t addressBytes,uint8_t dummyBytes,const void *data,uint32_t dataSize) const;
        bool doWriteCommandStart(uint8_t command,uint32_t address,uint8_t addressBytes,uint8_t dummyBytes) const;
        bool doReadCommand(uint8_t command,uint32_t address,uint8_t addressBytes,uint8_t dummyBytes,void *data,uint32_t dataSize) const;
    };


    /**
     * Write a command. No address or data
     * @param command The command byte to write
     * @return true if it worked
     */

    template<class TSpi>
    inline bool Command<TSpi>::writeCommand(uint8_t command) const {

      bool retval;

      // CS=low, write data, CS=high

      this->_spi->setNss(false);
      retval=this->_spi->send(&command,1);
      this->_spi->setNss(true);

      return retval;
    }


    /**
     * Write a command, with all options
     * @param command The command byte to write
     * @param address The address to write at
     * @param addressBytes The number of bytes that make up the address
     * @param dummyBytes The number of dummy (zero) bytes to write before the data
     * @param data Pointer to the data to send
     * @param dataSize The number of bytes to send
     * @return true if it worked
     */

    template<class TSpi>
    inline bool Command<TSpi>::writeCommand(uint8_t command,
                                            uint32_t address,
                                            uint8_t addressBytes,
                                            uint8_t dummyBytes,
                                            const void *data,
                                            uint32_t dataSize) const {

      bool retval;

      this->_spi->setNss(false);
      retval=doWriteCommand(command,address,addressBytes,dummyBytes,data,dataSize);
      this->_spi->setNss(true);

      return retval;
    }


    /*
     * Actually do a write while CS is low
     */

    template<class TSpi>
    inline bool Command<TSpi>::doWriteCommand(uint8_t command,
                                              uint32_t address,
                                              uint8_t addressBytes,
                                              uint8_t dummyBytes,
                                              const void *data,
                                              uint32_t dataSize) const {

      // write out the common starting sequence

      if(!doWriteCommandStart(command,address,addressBytes,dummyBytes))
        return false;

      // write out the data block

      if(dataSize)
        return this->_spi->send(reinterpret_cast<const uint8_t *>(data),dataSize);

      return true;
    }


    /*
     * Write out the common command,address,dummy starting sequence
     */

    template<class TSpi>
    inline bool Command<TSpi>::doWriteCommandStart(uint8_t command,
                                                   uint32_t address,
                                                   uint8_t addressBytes,
                                                   uint8_t dummyBytes) const {

      int i;
      uint8_t c;

      // write the command

      if(!this->_spi->send(&command,1))
        return false;

      // write the address bytes, MSB first

      if(addressBytes) {
        for(i=addressBytes-1;i>=0;i--) {
          c=address >> (8*i);
          if(!this->_spi->send(&c,1))
            return false;
        }
      }

      // if any dummy bytes, write them

      if(dummyBytes) {

        c=0;
        while(dummyBytes--) {
          if(!this->_spi->send(&c,1))
            return false;
        }
      }

      // done

      return true;
    }


    /**
     * Send a command to read data, with all options
     * @param command The command byte to write
     * @param address The address to read from
     * @param addressBytes The number of bytes that make up the address
     * @param dummyBytes The number of dummy (zero) bytes to write before the data
     * @param data Pointer to the data to receive
     * @param dataSize The number of bytes to receive
     * @return true if it worked
     */

    template<class TSpi>
    inline bool Command<TSpi>::readCommand(uint8_t command,
                                           uint32_t address,
                                           uint8_t addressBytes,
                                           uint8_t dummyBytes,
                                           void *data,
                                           uint32_t dataSize) const {

      bool retval;

      this->_spi->setNss(false);
      retval=doReadCommand(command,address,addressBytes,dummyBytes,data,dataSize);
      this->_spi->setNss(true);

      return retval;
    }


    /*
     * Actually do a read while CS is low
     */

    template<class TSpi>
    inline bool Command<TSpi>::doReadCommand(uint8_t command,
                                             uint32_t address,
                                             uint8_t addressBytes,
                                             uint8_t dummyBytes,
                                             void *data,
                                             uint32_t dataSize) const {

      // write out the common starting sequence

      if(!doWriteCommandStart(command,address,addressBytes,dummyBytes))
        return false;

      // read using the dummy write method

      return this->_spi->receive(reinterpret_cast<uint8_t *>(data),dataSize);
    }
  }
}
