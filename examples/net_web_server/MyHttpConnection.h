/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * Our connection class. The HTTP server is templated to return instances of this class when
 * remote clients connect. Each instance of this class independently handles an incoming
 * connection. This model allows you to easily service multiple connections without having
 * to worry about doing the multiplexing yourself. When you're done with a connection just
 * delete it and the event system will take care of all necessary close and cleanup.
 */

class MyHttpConnection : public HttpServerConnection<MyHttpConnection> {

	public:

		/**
		 * Customised parameters for this web server
		 */

		struct Parameters : HttpServerConnection<MyHttpConnection>::Parameters {
			Parameters() {
				tcp_receiveBufferSize=512;						// increased receive buffer to get the request in one segment
				http_outputStreamBufferMaxSize=2048;	// increased send buffer to push out larger segments to the browser
				http_version11=true;									// permit http/1.1 keep-alive to keep closing connection states down
			}
		};

	protected:
		FileSystem *_fs;
		File *_file;

	protected:
		void processRequest();
		void closeFile();
		FileInputStream *getErrorPageStream(const std::string& errorCode);

	public:
		MyHttpConnection(const Parameters& params,FileSystem *fs);
		~MyHttpConnection();

		bool handleClosed();
		bool handleCallback();
		State handleStateChange(State newState);
		void handleRequestHeader(const std::string&);
};


/**
 * Constructor. We need to supply the base class with its parameters and we stash
 * the filesystem object that will be used to serve the web files.
 */

inline MyHttpConnection::MyHttpConnection(const Parameters& params,FileSystem *fs)
	: HttpServerConnection<MyHttpConnection>(params),
	  _fs(fs),
	  _file(nullptr) {
}


/**
 * Destructor, free resources
 */

inline MyHttpConnection::~MyHttpConnection() {
	closeFile();
}


inline void MyHttpConnection::closeFile() {
	if(_file!=nullptr) {
		delete _file;
		_file=nullptr;
	}
}


/**
 * Callback for when the connection is detected to be closed. We will self-destruct which will
 * cause the connection-released event to be fired and the connection state will be transferred
 * back to the network stack where the TCP closing sequence is managed. The connection itself
 * will be automatically removed from the connections array.
 * @return true - no need to abort the connection round robin.
 */

inline bool MyHttpConnection::handleClosed() {
	delete this;
	return true;
}


/**
 * Round-robin opportunity to do some processing. We're not interested.
 * @return true
 */

inline bool MyHttpConnection::handleCallback() {
	return true;
}


/**
 * There was a state change in the HTTP connection
 * @param newState The proposed new state
 * @return The actual new state
 */

inline HttpServerConnection<MyHttpConnection>::State MyHttpConnection::handleStateChange(State newState) {

	// a move away from the read states and on to the first write state is our trigger to
	// process the request and make the response available

	if(newState==State::READING_REQUEST_LINE)
		closeFile();													// restarting an HTTP/1.1 connection
	if(newState==State::WRITING_RESPONSE)
		processRequest();

	return newState;
}


/**
 * We don't process headers
 * @param
 */

inline void MyHttpConnection::handleRequestHeader(const std::string&) {
}


/**
 * Process the incoming request
 */

inline void MyHttpConnection::processRequest() {

	std::string *response;
	FileInputStream *fis;

	fis=nullptr;

	if(!strcasecmp(_action.c_str(),"GET")) {

		if(_fs->openFile(_uri.c_str(),_file)) {

			response=new std::string(_version+" 200 OK\r\n");
			fis=new FileInputStream(*_file);
		}
		else {
			response=new std::string(_version+" 404 Not Found\r\n");
			fis=getErrorPageStream("404");
		}
	}
	else {
		response=new std::string(_version+" 501 Not Implemented\r\n");
		fis=getErrorPageStream("501");
	}

	// add headers

	addConnectionHeader(*response);

	if(fis) {
		addContentTypeHeader(*response);
		addContentLengthHeader(*response,_file->getLength());
	}

	(*response)+="\r\n";

	// add streams to response

	_output.addStream(new StlStringInputStream(response,true),true);

	if(fis)
		_output.addStream(fis,true);
}


/**
 * Get a new stream on to an error page. First /errors/<code>.html is checked and then /error.html
 * is checked. The URI is adjusted accordingly.
 * @return An input stream on to the file
 */

inline FileInputStream *MyHttpConnection::getErrorPageStream(const std::string& errorCode) {

	std::string filename("/errors/"+errorCode+".html");

	// first try the specific error page

	if(!_fs->openFile(filename.c_str(),_file)) {

		// now try the generic error page

		filename="/error.html";
		if(!_fs->openFile(filename.c_str(),_file))
			return nullptr;
	}

	// got it

	_uri=filename;
	return new FileInputStream(*_file);
}

