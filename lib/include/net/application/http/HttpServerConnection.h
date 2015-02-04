/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * HttpServerConnection provides a derivation of TcpConnection that understands how
     * to receive an HTTP request and transmit the response. The actual work of processing
     * the request is delegated to your subclass once the request has been received and
     * parsed. This template follows the CRTP pattern of you parameterising it with your
     * implementation.
     *
     * We support HTTP/1.1 and HTTP/1.0 connections. In HTTP/1.1 mode
     */

    template<class TImpl>
    class HttpServerConnection : public TcpConnection {

      public:

        /**
         * Parameters for this class
         */

        struct Parameters : TcpConnection::Parameters {

          bool http_version11;                      ///< are we operating in HTTP/1.1 mode? default is true.
          uint16_t http_maxRequestLineLength;       ///< size includes the verb, URL and HTTP version. Default is 200
          uint16_t http_outputStreamBufferMaxSize;  ///< buffer size of the stream-of-streams class. Default is 256
          uint16_t http_maxRequestsPerConnection;   ///< in http1.1, close connection after this many requests. 0 = never, default is 5.

          Parameters() {
            http_version11=false;
            http_maxRequestLineLength=200;
            http_outputStreamBufferMaxSize=256;
            http_maxRequestsPerConnection=5;
          }
        };

      protected:
        const Parameters& _params;          ///< reference to the parameters class
        uint32_t _contentLength;            ///< value of the Content-Length header
        OutputStream *_requestBody;         ///< derivation sets this non-null when it wants the request body
        uint32_t _responseSize;             ///< derivation sets this non-zero along with response body so Content-Length header can be sent back to client
        TcpTextLineReceiver _currentLine;   ///< current line that we're processing
        TcpOutputStreamOfStreams _output;   ///< the output streams that form the response
        uint16_t _requestsServed;           ///< count of requests served so far
        std::string _version;
        std::string _action;
        std::string _uri;

        /**
         * States that we transition through while processing a request
         */

        enum class State : uint8_t {

          READING_REQUEST_LINE,         ///< Reading the first line, e.g. GET /index.html HTTP/1.1
          READING_REQUEST_HEADERS,      ///< Reading the request headers
          READING_REQUEST_BODY,         ///< Reading the request body
          WRITING_BEGIN,                ///< Writing is ready to start
          WRITING_RESPONSE,             ///< Writing back the response from the stream of streams
        } _state;

      protected:
        HttpServerConnection(const Parameters& params);

        void changeState(State newState);
        void processRequestHeader(const std::string& header);

        void addConnectionHeader(std::string& response);
        void addContentTypeHeader(std::string& response);
        void addContentTypeHeader(std::string& response,const char *contentType);
        void addContentLengthHeader(std::string& response,uint32_t contentLength);

        void readRequestLine();
        void readRequestHeaders();
        void readRequestBody();
        void parseRequestLine();
        std::string decodeUri(const std::string& uri);

      public:
        bool handleRead();              ///< implementation requirement from the TcpConnectionArray
        bool handleWrite();             ///< implementation requirement from the TcpConnectionArray
    };


    /**
     * Constructor
     */

    template<class TImpl>
    inline HttpServerConnection<TImpl>::HttpServerConnection(const Parameters& params)
      : TcpConnection(params),
        _params(params),
        _contentLength(0),
        _requestBody(nullptr),
        _responseSize(0),
        _currentLine(params.http_maxRequestLineLength),
        _output(*this,params.http_outputStreamBufferMaxSize),
        _requestsServed(0),
        _state(State::READING_REQUEST_LINE) {
    }


    /**
     * Some data is ready for reading
     * @return true always: we don't want to ever abandon the client's wait() call
     */

    template<class TImpl>
    inline bool HttpServerConnection<TImpl>::handleRead() {

      while(getDataAvailable()) {

        switch(_state) {

          case State::READING_REQUEST_LINE:       // first line of a request. contains the verb, URL, http version
            readRequestLine();
            break;

          case State::READING_REQUEST_HEADERS:
            readRequestHeaders();
            break;

          case State::READING_REQUEST_BODY:
            readRequestBody();
            break;

          default:                // not a read state
            return true;
        }
      }

      return true;
    }


    /**
     * Read the request line
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::readRequestLine() {

      _currentLine.add(*this);

      if(_currentLine.ready()) {
        parseRequestLine();
        changeState(State::READING_REQUEST_HEADERS);
        _currentLine.reset();
      }
    }


    /**
     * Parse the request line: ACTION SP URI SP PROTOCOL
     * e.g. GET http://www.foo.com/this/file.html HTTP/1.1
     * e.g. GET /this/file.html HTTP/1.1
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::parseRequestLine() {

      std::string::size_type pos,pos2;
      const std::string& request(_currentLine.str());

      if((pos=request.find(' '))!=std::string::npos) {

        // first the action

        _action=request.substr(0,pos);

        if((pos2=request.find(' ',pos+1))!=std::string::npos) {

          // now URI and protocol

          _uri=decodeUri(request.substr(pos+1,pos2-pos-1));
          _version=request.substr(pos2+1);

          // URI may be absolute. deal with that.

          if((pos=_uri.find("://"))!=std::string::npos) {

            // found the abs prefix, find the URI at the end

            if((pos=_uri.find('/',pos+3))!=std::string::npos)
              _uri=_uri.substr(pos);      // URI found, keep it
            else
              _uri="/index.html";         // only host and protocol found, default to /index.html
          }
        }
      }
    }


    /**
     * Read the request headers
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::readRequestHeaders() {

      _currentLine.add(*this);

      if(_currentLine.ready()) {

        if(_currentLine.length()==0) {
          if(_contentLength>0)
            changeState(State::READING_REQUEST_BODY);                       // there's a body to read
          else
            changeState(State::WRITING_BEGIN);                              // zero length = request complete
        }
        else {
          processRequestHeader(_currentLine);                               // got a header, see if we care about it
          static_cast<TImpl *>(this)->handleRequestHeader(_currentLine);    // notify the subclass
        }

        _currentLine.reset();                                               // ready for reading next header
      }
    }


    /**
     * Read the request body
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::readRequestBody() {

      uint8_t buf[100];
      uint32_t size,actuallyReceived;

      // loop until we've read all

      while(_contentLength && getDataAvailable()) {

        // read a chunk

        size=std::min((uint32_t)sizeof(buf),_contentLength);

        if(!receive(buf,size,actuallyReceived,0))
          return;

        // write or discard

        if(_requestBody)
          _requestBody->write(buf,actuallyReceived);

        // reduce data to read

        _contentLength-=size;
      }
    }


    /**
     * Some data is ready for writing
     * @return true always: we don't want to ever abandon the client's wait() call
     */

    template<class TImpl>
    inline bool HttpServerConnection<TImpl>::handleWrite() {

      if(_state==State::WRITING_BEGIN) {

        // WRITING_BEGIN exists so that a subclass can detect a state change from WRITING_BEGIN
        // to WRITING_RESPONSE and use it to open the resource demanded by the request. The subclass
        // should not use the state change *to* WRITING_BEGIN because that may happen in IRQ code
        // if fast reading is enabled.

        changeState(State::WRITING_RESPONSE);
      }
      else if(_state==State::WRITING_RESPONSE) {

        uint32_t actuallySent;

        if(!_output.writeDataToConnection(actuallySent)) {        // try to write data and kill the connection if it fails

          delete this;
          return true;
        }

        // any more data to write?

        if(_output.completed()) {

          _requestsServed++;

          // if this is not HTTP/1.1 or we have served the max number of connections then close the connection

          if(strcasecmp(_version.c_str(),"HTTP/1.1")!=0 ||
             !_params.http_version11 ||
             (_params.http_maxRequestsPerConnection && _requestsServed>=_params.http_maxRequestsPerConnection)) {

            delete this;
            return true;
          }

          // reset and move on to next request from client

          _contentLength=0;

          _action.clear();
          _uri.clear();
          _version.clear();

          changeState(State::READING_REQUEST_LINE);
        }
      }

      return true;
    }


    /**
     * Notify the subclass of a state change. The subclass can opt to change the next state.
     * @param newState the new state. The subclass can read _state to determine the old (current) state
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::changeState(State newState) {
      _state=static_cast<TImpl *>(this)->handleStateChange(newState);
    }


    /**
     * Process an incoming request header
     * @param header The header, as supplied by the client
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::processRequestHeader(const std::string& header) {

      std::string::size_type pos;

      // we only care about Content-Length since the client may be doing a POST

      if(!strcasecmp(header.c_str(),"Content-Length:")) {
        for(pos=15;pos!=header.length() && isspace(header[pos]);pos++);
        if(pos!=header.length())
          _contentLength=atoi(header.c_str()+pos);
      }
    }


    /**
     * Add the Connection: close/keep-alive header
     * @param response The response string
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::addConnectionHeader(std::string& response) {

      if(!strcasecmp(_version.c_str(),"HTTP/1.1") &&
          _params.http_version11 &&
          (_params.http_maxRequestsPerConnection==0 || _requestsServed!=_params.http_maxRequestsPerConnection-1))
        response+="Connection: keep-alive\r\n";
      else
        response+="Connection: close\r\n";
    }


    /**
     * Try to add a content-type header from a limited set of known types
     * @param response The response string
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::addContentTypeHeader(std::string& response) {

      std::string::size_type pos;
      const char *ext;

      if((pos=_uri.find_last_of('.'))==std::string::npos)
        return;       // not found

      ext=&_uri.c_str()[pos+1];

      if(!strcasecmp(ext,"htm") || !strcasecmp(ext,"html"))
        addContentTypeHeader(response,"text/html");
      else if(!strcasecmp(ext,"js"))
        addContentTypeHeader(response,"application/javascript");
      else if(!strcasecmp(ext,"jpg"))
        addContentTypeHeader(response,"image/jpeg");
      else if(!strcasecmp(ext,"gif"))
        addContentTypeHeader(response,"image/gif");
      else if(!strcasecmp(ext,"png"))
        addContentTypeHeader(response,"image/png");
      else if(!strcasecmp(ext,"pdf"))
        addContentTypeHeader(response,"application/pdf");
      else if(!strcasecmp(ext,"txt"))
        addContentTypeHeader(response,"text/plain");
      else if(!strcasecmp(ext,"css"))
        addContentTypeHeader(response,"text/css");
    }


    /**
     * Add the given content type
     * @param response The response string
     * @param contentType The content type
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::addContentTypeHeader(std::string& response,const char *contentType) {
      response+="Content-Type: ";
      response+=contentType;
      response+="\r\n";
    }


    /**
     * Add the given content length
     * @param response The response string
     * @param contentLength The content length
     */

    template<class TImpl>
    inline void HttpServerConnection<TImpl>::addContentLengthHeader(std::string& response,uint32_t contentLength) {

      char buffer[20];

      StringUtil::modp_uitoa10(contentLength,buffer);
      response+="Content-Length: ";
      response+=buffer;
      response+="\r\n";
    }


    /**
     * Decode the URI by replacing %xx chars
     * @param uri
     * @return
     */

    template<class TImpl>
    inline std::string HttpServerConnection<TImpl>::decodeUri(const std::string& uri) {

      std::string ret;
      std::string::size_type i;
      uint8_t ch;

      ret.reserve(uri.length());

      for(i=0;i<uri.length();i++) {

        if(uri[i]=='%' && i<=uri.length()-3) {

          ch=((uri[i+1]-'0')*16)+(uri[i+2]-'0');
          ret+=static_cast<char>(ch);
          i+=2;
        }
        else
          ret+=uri[i];
      }
      return ret;
    }
  }
}
