/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * TcpOutputStreamOfStreams manages a list of streams that should be sequentially
     * pushed out over a TCP connection. Each input stream can be optionally deleted
     * when it's empty, or when this class is destroyed. The write methods are non-blocking.
     * If a call would block then an error code is returned.
     *
     * Because streams cannot be rewound and the TCP connection can write less than you
     * ask for, a local buffer is maintained that holds data read from the stream but not
     * yet sent to the TCP connection. The default size of this buffer is 256 bytes but
     * that can be configured in the constructor. This buffer size has an important impact
     * on performance because it will correspond to the largest TCP data segment that this
     * class will send over the connection.
     */

    class TcpOutputStreamOfStreams {

      protected:
        typedef std::pair<InputStream *,bool> StreamEntry;
        std::list<StreamEntry> _streamList;
        TcpConnection& _conn;
        scoped_array<uint8_t> _localBuffer;
        uint16_t _localBufferPos;
        uint16_t _localBufferMaxSize;
        uint16_t _localBufferSize;

      protected:
        bool fillLocalBuffer();

      public:
        TcpOutputStreamOfStreams(TcpConnection& conn,uint16_t localBufferMaxSize=256);
        ~TcpOutputStreamOfStreams();

        bool completed() const;
        void addStream(InputStream *stream,bool takeOwnership);
        bool canWriteToConnection() const;
        bool writeDataToConnection(uint32_t& actuallySent);
    };


    /**
     * Constructor
     * @param The TCP connection to write to.
     */

    inline TcpOutputStreamOfStreams::TcpOutputStreamOfStreams(TcpConnection& conn,uint16_t localBufferMaxSize)
      : _conn(conn),
        _localBuffer(new uint8_t[localBufferMaxSize]),
        _localBufferPos(0),
        _localBufferMaxSize(localBufferMaxSize),
        _localBufferSize(0) {
    }


    /**
     * Destructor. Delete any streams that we have taken ownership of
     */

    inline TcpOutputStreamOfStreams::~TcpOutputStreamOfStreams() {

      // delete all entries in the list that we own

      for(auto it=_streamList.begin();it!=_streamList.end();it++)
        if(it->second)
          delete it->first;
    }


    /**
     * Add a stream to the list, optionally owning it
     * @param stream The stream pointer
     * @param takeOwnership true if we own the pointer and will delete it when finished
     * @return
     */

    inline void TcpOutputStreamOfStreams::addStream(InputStream *stream,bool takeOwnership) {
      _streamList.push_back(StreamEntry(stream,takeOwnership));
    }


    /**
     * Return true if we can write to this connection without blocking
     * @return true if we can do the write
     */

    inline bool TcpOutputStreamOfStreams::canWriteToConnection() const {
      return _conn.getTransmitWindowSize()>0;
    }


    /**
     * Write out some data to the connection. Will never block. Will return false
     * if the underlying connection write attempt or a stream read attempt returned
     * false. If this method returns true then zero or more bytes have been transmitted.
     * @param actuallySent The number of bytes sent.
     * @return true if it worked.
     */

    inline bool TcpOutputStreamOfStreams::writeDataToConnection(uint32_t& actuallySent) {

      // reset this

      actuallySent=0;

      // if the local buffer is empty, get some data

      if(_localBufferPos==_localBufferSize)
        if(!fillLocalBuffer())
          return false;

      // if the local buffer is still empty then there is no data to write

      if(_localBufferPos==_localBufferSize)
        return true;

      // write out the data from the buffer with no blocking

      if(!_conn.send(&_localBuffer[_localBufferPos],_localBufferSize-_localBufferPos,actuallySent,0))
        return false;

      // update position and return

      _localBufferPos+=actuallySent;
      return true;
    }


    /**
     * Fill the local buffer with data to send
     * @return false if a stream failed
     */

    inline bool TcpOutputStreamOfStreams::fillLocalBuffer() {

      uint16_t pos,toread;
      uint32_t actuallyRead;

      // it's empty now

      _localBufferSize=0;
      _localBufferPos=0;
      pos=0;

      // if there are no streams then then we cannot read any data - not an error

      if(_streamList.size()==0)
        return true;

      StreamEntry se(*_streamList.begin());

      while(_localBufferSize!=_localBufferMaxSize) {

        // try to read to the end

        toread=_localBufferMaxSize-_localBufferSize;

        // read the data and return false if the stream fails

        if(!se.first->read(&_localBuffer[pos],toread,actuallyRead))
          return false;

        if(actuallyRead==0) {

          // no data was read, need to move to the next stream

          if(se.second)
            delete se.first;        // we own the stream, delete it

          // remove it from the list

          _streamList.erase(_streamList.begin());

          // get out if the list is empty

          if(_streamList.size()==0)
            return true;

          // move to the next stream

          se=*_streamList.begin();
        }
        else {
          _localBufferSize+=actuallyRead;
          pos+=actuallyRead;
        }
      }

      return true;
    }

    /**
     * Return true if all data has gone
     * @return true if all data has gone
     */

    inline bool TcpOutputStreamOfStreams::completed() const {
      return _streamList.size()==0 && _localBufferPos==_localBufferSize;
    }
  }
}
