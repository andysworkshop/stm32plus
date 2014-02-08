/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    class HttpClient {

      public:

        /**
         * Error codes
         */

        enum {
          E_INVALID_METHOD,     ///< only GET and POST are supported
          E_INVALID_RESPONSE,   ///< could not parse the first line of the response
          E_TIMED_OUT           ///< timed out while sending/receiving
        };

      protected:
        TcpConnection& _conn;                       ///< reference to the TCP connection

        std::slist<std::string> _requestHeaders;    ///< headers to set on the request
        HttpVersion _httpVersion;                   ///< protocol version, default is 1.1
        HttpMethod _httpMethod;                     ///< method, default is GET
        std::string _uri;                           ///< URI for the request (no host and no protocol)
        std::string _host;                          ///< the Host: header value (mandatory for HTTP/1.1)

        uint16_t _maxResponseHeaderLineLength;      ///< maximum length of a response header line before we truncate it. default is 100.
        std::slist<std::string> _responseHeaders;   ///< response header list
        uint16_t _responseCode;                     ///< HTTP response code number
        int32_t _responseContentLength;             ///< content length of response, or -1 if server not sent
        std::string _responseContentType;           ///< response content type, or empty if server not sent

      public:
        HttpClient(TcpConnection& conn);

        bool sendRequest(uint32_t timeoutMillis=0);
        bool readResponse(uint32_t timeoutMillis=0);

        void setVersion(HttpVersion version);
        void setMethod(HttpMethod method);
        void setUri(const std::string& uri);
        void setHost(const std::string& host);
        void setRequestContentType(const std::string& contentType);
        void setRequestContentLength(uint32_t contentLength);

        uint16_t getResponseCode() const;
        int32_t getResponseContentLength() const;
        const std::string& getResponseContentType() const;
        const std::slist<std::string>& getResponseHeaders() const;
    };


    /**
     * Constructor, initialise the stream constructors and some defaults
     * @param The TCP connection to use
     */

    inline HttpClient::HttpClient(TcpConnection& conn)
      : _conn(conn) {

      _httpVersion=HttpVersion::HTTP_1_1;
      _httpMethod=HttpMethod::GET;
      _maxResponseHeaderLineLength=100;
    }


    /**
     * Convenience method to set the Content-Type header for POST-type requests
     * @param contentType The content type string
     */

    inline void HttpClient::setRequestContentType(const std::string& contentType) {
      _requestHeaders.push_front("Content-Type: "+contentType);
    }


    /**
     * Set the Content-Length header for POST-type requests
     * @param contentLength The content length, in bytes.
     */

    inline void HttpClient::setRequestContentLength(uint32_t contentLength) {

      char buffer[30];

      strcpy(buffer,"Content-Length: ");
      StringUtil::modp_uitoa10(contentLength,buffer+16);

      _requestHeaders.push_front(buffer);
    }


    /**
     * Set the http version
     * @param version The http version
     */

    inline void HttpClient::setVersion(HttpVersion version) {
      _httpVersion=version;
    }


    /**
     * Set the HTTP method
     * @param method The HTTP method
     */

    inline void HttpClient::setMethod(HttpMethod method) {
      _httpMethod=method;
    }


    /**
     * Set the HTTP URI
     * @param uri The URI
     */

    inline void HttpClient::setUri(const std::string& uri) {
      _uri=uri;
    }


    /**
     * Set the HTTP host (mandatory for HTTP/1.1)
     * @param host The host
     */

    inline void HttpClient::setHost(const std::string& host) {
      _host=host;
    }


    /**
     * Connect to the remote server, send the request line and headers.
     *
     * If this is a GET request then the response code and response content length will be
     * available to read and then the methods of the TcpConnection and TcpInputStream parents
     * are available to read the response body.
     *
     * If this is a request that requires a body (e.g. POST) then the methods of TcpConnection
     * and TcpOutputStream can be used to send the post content. When the content has been sent
     * you must manually call readResponse().
     * You can then use the methods of TcpConnection and TcpInputStream to read the response body.
     *
     * @param timeoutMillis how long to wait for a send or receive to complete (0=blocking), default is zero.
     * @return true if it worked, false if there was a network error
     */

    inline bool HttpClient::sendRequest(uint32_t timeoutMillis) {

      // embed this in a subcontext so the request string is destructed before
      // the automatic call to readResponse

      {
        std::string request;
        uint32_t actuallySent;

        // all the methods are supported

        if(_httpMethod==HttpMethod::POST)
          request="POST ";
        else if(_httpMethod==HttpMethod::GET)
          request="GET ";
        else if(_httpMethod==HttpMethod::HEAD)
          request="HEAD ";
        else if(_httpMethod==HttpMethod::PUT)
          request="PUT ";
        else if(_httpMethod==HttpMethod::DELETE)
          request="DELETE ";
        else if(_httpMethod==HttpMethod::TRACE)
          request="TRACE ";
        else if(_httpMethod==HttpMethod::CONNECT)
          request="CONNECT ";
        else
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_HTTP_CLIENT,E_INVALID_METHOD);

        // add the URI

        request+=_uri;

        // add the version

        if(_httpVersion==HttpVersion::HTTP_1_0)
          request+=" HTTP/1.0\r\n";
        else
          request+=" HTTP/1.1\r\n";

        // add the Host header

        request+="Host: "+_host+"\r\n";

        // add user headers

        for(auto it=_requestHeaders.begin();it!=_requestHeaders.end();it++)
          request+=*it+"\r\n";

        // add the termination blank line

        request+="\r\n";

        // send it to the server

        if(!_conn.send(request.c_str(),request.length(),actuallySent,timeoutMillis) || actuallySent!=request.length())
          return false;
      }

      // if this was a GET then automatically read the response

      if(_httpMethod==HttpMethod::GET)
        return readResponse(timeoutMillis);

      return true;
    }


    /**
     * Read the response from the server up to and including the headers. The response code is made available
     * by calling getResponseCode() and the headers by calling getResponseHeaders(). You only call this method
     * if you are using a POST request. sendRequest() calls it automatically for GET requests.
     *
     * @param timeoutMillis how long to wait for a send or receive to complete (0=blocking), default is zero.
     * @return true if it worked, false if there was a network error
     */

    inline bool HttpClient::readResponse(uint32_t timeoutMillis) {

      TcpTextLineReceiver lineReceiver(_maxResponseHeaderLineLength);
      std::string::size_type pos1,pos2;
      uint32_t now;

      if(timeoutMillis)
        now=MillisecondTimer::millis();
      else
        now=0;

      // read the response line

      while(!lineReceiver.ready()) {

        if(!lineReceiver.add(_conn))
          return false;

        if(timeoutMillis && MillisecondTimer::hasTimedOut(now,timeoutMillis))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_HTTP_CLIENT,E_TIMED_OUT);
      }

      // get the status code from the response line

      const std::string& statusLine(lineReceiver.str());

      if((pos1=statusLine.find(' '))==std::string::npos || (pos2=statusLine.find(' ',pos1+1))==std::string::npos)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_HTTP_CLIENT,E_INVALID_RESPONSE);

      _responseCode=atoi(statusLine.substr(pos1+1,pos2-pos1+1).c_str());

      // read the headers

      _responseHeaders.clear();
      _responseContentType.clear();
      _responseContentLength=-1;

      for(;;) {

        // reset the line

        lineReceiver.reset();

        // read the next line

        while(!lineReceiver.ready()) {

          if(!lineReceiver.add(_conn))
            return false;

          if(timeoutMillis && MillisecondTimer::hasTimedOut(now,timeoutMillis))
            return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_HTTP_CLIENT,E_TIMED_OUT);
        }

        // finished?

        const std::string& headerLine(lineReceiver.str());
        if(headerLine.length()==0)
          break;

        if((pos1=headerLine.find(':'))!=std::string::npos) {

          // advance past white space

          for(pos1++;pos1!=headerLine.length() && isspace(headerLine[pos1]);pos1++);

          if(pos1!=headerLine.length()) {

            // check for content-type and content-length

            if(!strncasecmp(headerLine.c_str(),"Content-Type",12))
              _responseContentType=headerLine.substr(pos1);
            else if(!strncasecmp(headerLine.c_str(),"Content-Length",14))
              _responseContentLength=atol(headerLine.substr(pos1).c_str());
          }

          // add the header line

          _responseHeaders.push_front(lineReceiver.str());
        }
      }

      // we've read the headers

      return true;
    }


    /**
     * Return the last response code
     * @return The response code
     */

    inline uint16_t HttpClient::getResponseCode() const {
      return _responseCode;
    }


    /**
     * Return the last response headers
     * @return The last response headers
     */

    inline const std::slist<std::string>& HttpClient::getResponseHeaders() const {
      return _responseHeaders;
    }


    /**
     * Return the response content length. The value is -1 if the content length was
     * not provided.
     * @return The content length, or -1 if the server did not set it
     */

    inline int32_t HttpClient::getResponseContentLength() const {
      return _responseContentLength;
    }


    /**
     * Get the response content type, or empty string if not set by the server
     * @return The content type
     */

    inline const std::string& HttpClient::getResponseContentType() const {
      return _responseContentType;
    }
  }
}
