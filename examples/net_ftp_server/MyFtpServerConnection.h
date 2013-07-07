/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * Our command connection class. The FTP server is templated to return instances of this
 * class when remote clients connect. Each instance of this class independently handles an
 * incoming connection. This model allows you to easily service multiple connections without
 * having to worry about doing the multiplexing yourself. When you're done with a connection
 * just delete it and the event system will take care of all necessary close and cleanup.
 */


class MyFtpServerConnection : public FtpServerConnection<MyFtpServerConnection,Tcp<MyNetworkLayer> > {

	public:

		typedef FtpServerConnection<MyFtpServerConnection,Tcp<MyNetworkLayer> > FtpServerConnectionType;

		/**
		 * Customised parameters for this ftp server
		 */

		struct Parameters : FtpServerConnectionType::Parameters {

			/**
			 * Constructor
			 */

			Parameters() {

				// this is an interactive connection so switch off the nagle avoidance and enable PUSH

				tcp_nagleAvoidance=false;
				tcp_push=true;
			}
		};

	protected:
		FtpParameters *_ftpParameters;
		uint16_t _userIndex;
		std::string _cwd;

	protected:
		std::string appendPath(const std::string& prefix,const std::string& path) const;
		bool normalisePath(std::string& path) const;
		std::string getCurrentDirectory() const;
		bool directoryListing(void (MyFtpServerConnection::*printFunction)(const FileInformation&));
		void printFileDetails(const FileInformation& finfo);
		void printFileName(const FileInformation& finfo);
		bool getPaths(const char *dir,std::string& subdir,std::string& fullpath);

	public:
		MyFtpServerConnection(const Parameters& params,FtpParameters *ftpParameters);

		// required by the parent

		bool handleClosed();
		bool writeGreeting();
		bool writeGoodbye();
		bool isAnonymousPermitted();
		bool loginUser(const char *user,const char *password);
		bool setCwd(const char *cwd);
		bool download(const char *filename);
		const char *getCwd();
		bool simpleListing();
		bool complexListing();
		bool createDirectory(const char *dir,std::string& newdir);
		bool removeDirectory(const char *dir);
		bool removeFile(const char *filename);
		bool upload(const char *param,bool append,OutputStream*& stream);
		bool fileSize(const char *filename,uint32_t& fileSize);
};


/**
 * Constructor. We need to supply the base class with its parameters and we stash
 * the ftp parameters object that will be used to serve the web files.
 */

inline MyFtpServerConnection::MyFtpServerConnection(const Parameters& params,FtpParameters *ftpParameters)
	: FtpServerConnectionType(params,ftpParameters->tcpImpl),
	  _ftpParameters(ftpParameters) {
}


/**
 * Callback for when the connection is detected to be closed. We will self-destruct which will
 * cause the connection-released event to be fired and the connection state will be transferred
 * back to the network stack where the TCP closing sequence is managed. The connection itself
 * will be automatically removed from the connections array.
 * @return true - no need to abort the connection round robin.
 */

inline bool MyFtpServerConnection::handleClosed() {
	delete this;
	return true;
}


/**
 * Check if anonymous is permitted
 * @return true if anonymous is permitted
 */

inline bool MyFtpServerConnection::isAnonymousPermitted() {
	return _ftpParameters->anonymousPermitted;
}


/**
 * Write the greeting message to the client
 * @return true to continue, false to close
 */

inline bool MyFtpServerConnection::writeGreeting() {

	// add the greeting to the outputs. the greeting already contains the correct 220 code prefixes

	_outputStreams.addStream(new StlStringInputStream(&_ftpParameters->serverGreeting,false),true);
	return true;
}


/**
 * Write the greeting message to the client
 * @return true to continue, false to close
 */

inline bool MyFtpServerConnection::writeGoodbye() {

	// add the goodbye to the outputs. the goodbye already contains the correct 221 code prefixes

	_outputStreams.addStream(new StlStringInputStream(&_ftpParameters->serverGoodbye,false),true);
	return true;
}


/**
 * Try to login the user given the credentials
 * @param user The user name
 * @param password The password
 * @return true if the user was logged in
 */

inline bool MyFtpServerConnection::loginUser(const char *user,const char *password) {

	// find the user

	for(auto it=_ftpParameters->users.begin();it!=_ftpParameters->users.end();it++) {

		if(it->name==user) {

			SHA1HashPeripheral<> sha1;
			std::string creds;
			uint8_t hash[20];
			char hex[41];

			// hash the salt and password concatenated

			creds=it->passwordSalt+password;
			sha1.hash(creds.c_str(),creds.length(),hash);

			// convert the hash to hex

			StringUtil::toHex(hash,20,hex);
			hex[40]='\0';

			// must match

			if(it->passwordHash!=hex)
				return false;

			_userIndex=it-_ftpParameters->users.begin();
			return true;
		}
	}

	// failed to find the user

	return false;
}


/**
 * The CWD is later appended to the server root to form the full path
 *
 * @param pathname The pathname to check. It's already checked for non-null
 * @return true if it's OK
 */

inline bool MyFtpServerConnection::setCwd(const char *pathname) {

	std::string newdir,fullpath;
	scoped_ptr<FileInformation> finfo;

	if(!getPaths(pathname,newdir,fullpath))
		return false;

	// get the file/directory information

	if(!_ftpParameters->fs->getFileInformation(fullpath.c_str(),finfo.address()))
		return false;

	if((finfo->getAttributes() & FileInformation::ATTR_DIRECTORY)==0)
		return false;

	_cwd=newdir;
	return true;
}


/**
 * Get the current working directory
 * @return
 */

inline const char *MyFtpServerConnection::getCwd() {
	return _cwd.c_str();
}


/**
 * Process a path so that .. navigation parts are removed
 * @param path The path to normalise
 * @return false if the path is invalid
 */

inline bool MyFtpServerConnection::normalisePath(std::string& path) const {

	std::vector<std::string> names,normalised;

	// break out the component parts

	StdStringUtil::tokenise(path,"/\\",names);

	for(auto it=names.begin();it!=names.end();it++) {

		// skip empty parts e.g. foo//bar has an empty part at position 1.

		if(it->length() && *it!=".") {

			if(*it=="..") {

				if(normalised.size()==0)
					return false;

				// remove the last part

				normalised.pop_back();
			}
			else
				normalised.push_back(*it);
		}
	}

	// put it back together

	path.clear();

	for(auto it=normalised.begin();it!=normalised.end();it++) {
		if(it!=normalised.begin())
			path+='/';

		path+=*it;
	}

	return true;
}


/**
 * Append path to prefix to get a new path
 * @param prefix The prefix
 * @param path The path
 * @return The new path
 */

inline std::string MyFtpServerConnection::appendPath(const std::string& prefix,const std::string& path) const {

	std::string newpath(path),newprefix(prefix);

	StdStringUtil::trimRight(newprefix,"/");
	StdStringUtil::trimLeft(newpath,"/");

	return newprefix+"/"+newpath;
}


/**
 * Get the current directory
 * @return The full directory including the root, '/' and the current working directory
 */

inline std::string MyFtpServerConnection::getCurrentDirectory() const {

	// _cwd has already been sanitised

	std::string dir(_ftpParameters->rootDirectory);

	if(!_cwd.empty()) {
		dir+='/';
		dir+=_cwd;
	}

	return dir;
}


/**
 * Send a simple directory listing to the data connection
 * @return true if it worked
 */

inline bool MyFtpServerConnection::simpleListing() {
	return directoryListing(&MyFtpServerConnection::printFileName);
}


/**
 * Send a complete listing to the data connection
 * @return true if it worked
 */

inline bool MyFtpServerConnection::complexListing() {
	return directoryListing(&MyFtpServerConnection::printFileDetails);
}


/**
 * Do a directory listing
 * @return true if it worked
 */

inline bool MyFtpServerConnection::directoryListing(void (MyFtpServerConnection::*printFunction)(const FileInformation& finfo)) {

	std::string fullPath(getCurrentDirectory());
	scoped_ptr<DirectoryIterator> dit;

	// get a directory iterator

	if(!_ftpParameters->fs->getDirectoryIterator(fullPath.c_str(),dit.address()))
		return false;

	// list the names

	while(dit->next()) {

		// get a reference to the FileInformation object that describes the file/directory
		// that we're currently looking at

		const FileInformation& fileInfo(dit->current());

		// write out the filename, ignoring "." and ".."

		if(strcmp(".",fileInfo.getFilename())!=0 && strcmp("..",fileInfo.getFilename())!=0)
			(this->*printFunction)(fileInfo);
	}

	return true;
}


/**
 * Print out just the filename
 */

inline void MyFtpServerConnection::printFileName(const FileInformation& finfo) {
	_dataConnection->addString(finfo.getFilename());
}


/**
 * Print out all file information
 */

inline void MyFtpServerConnection::printFileDetails(const FileInformation& finfo) {

	std::string str;
	char buffer[20];
	uint32_t len;

	// permissions string

	if((finfo.getAttributes() & FileInformation::ATTR_DIRECTORY)==0)
		str="-";
	else
		str="d";

	if((finfo.getAttributes() & FileInformation::ATTR_READ_ONLY)==0)
		str+="r-xr-xr-x";
	else
		str+="rwxrwxrwx";

	// owner/group (n/a)

	str+="   1      owner      group ";

	// size (pad to 10)

	len=StringUtil::modp_uitoa10(finfo.getLength(),buffer);
	memset(buffer+len,' ',10-len);
	buffer[10]='\0';

	str+=buffer;

	// date (fake this)

	str+=" Jan  1  1980 ";
	str+=finfo.getFilename();

	// add to the output

	_dataConnection->addString(str.c_str());
}


/**
 * Download the requested file
 * @param filename The file (which can be a pathname)
 */

inline bool MyFtpServerConnection::download(const char *filename) {

	std::string newdir,fullpath;
	File *newFile;

	if(!getPaths(filename,newdir,fullpath))
		return false;

	// the file must not be a directory

	{
		scoped_ptr<FileInformation> finfo;

		if(!_ftpParameters->fs->getFileInformation(fullpath.c_str(),finfo.address()))
			return false;

		if((finfo->getAttributes() & FileInformation::ATTR_DIRECTORY)!=0)
			return false;
	}

	// open the file

	if(!_ftpParameters->fs->openFile(fullpath.c_str(),newFile))
		return false;

	// if the client used REST then seek accordingly

	if(_sendStartPosition!=0) {
		if(!newFile->seek(_sendStartPosition,File::SeekStart)) {
			delete newFile;
			return false;
		}
	}

	// add an input stream on to the file that will take ownership of the file pointer

	_dataConnection->addStream(new OwnedFileInputStream(newFile),true);
	return true;
}


/**
 * Start the upload process
 * @param filename The file (which can be a pathname)
 * @param append true to append, false to overwrite/create
 * @param[out] stream The stream to write data to
 */

inline bool MyFtpServerConnection::upload(const char *filename,bool append,OutputStream *&stream) {

	std::string newdir,fullpath;
	File *newFile;

	// user must have write access

	if(!_ftpParameters->users[_userIndex].writeAccess)
		return false;

	if(!getPaths(filename,newdir,fullpath))
		return false;

	if(!append) {

		// delete if exists (ignore error, will be caught in next line)

		_ftpParameters->fs->deleteFile(fullpath.c_str());

		// create the file

		if(!_ftpParameters->fs->createFile(fullpath.c_str()))
			return false;
	}

	// open the file

	if(!_ftpParameters->fs->openFile(fullpath.c_str(),newFile))
		return false;

	// if we are appending, seek to the end

	if(append && !newFile->seek(0,File::SeekEnd))
		return false;

	// add an output stream on to the file that will take ownership of the file pointer
	// it's now the caller's responsibility to look after it

	stream=new OwnedFileOutputStream(newFile);
	return true;
}


/**
 * Create a new directory
 * @param dir
 * @param newdir
 * @return true if it worked
 */

inline bool MyFtpServerConnection::createDirectory(const char *dir,std::string& newdir) {

	std::string fullpath;

	// user must have write access

	if(!_ftpParameters->users[_userIndex].writeAccess)
		return false;

	if(!getPaths(dir,newdir,fullpath))
		return false;

	// try to create the directory

	return _ftpParameters->fs->createDirectory(fullpath.c_str());
}


/**
 * Remove a directory
 * @param dir
 * @return true if it worked
 */

inline bool MyFtpServerConnection::removeDirectory(const char *dir) {

	std::string fullpath,newdir;

	// user must have write access

	if(!_ftpParameters->users[_userIndex].writeAccess)
		return false;

	if(!getPaths(dir,newdir,fullpath))
		return false;

	// try to create the directory

	return _ftpParameters->fs->deleteDirectory(fullpath.c_str());
}


/**
 * Remove a file
 * @param filename
 * @return true if it worked
 */

inline bool MyFtpServerConnection::removeFile(const char *filename) {

	std::string fullpath,newdir;

	// user must have write access

	if(!_ftpParameters->users[_userIndex].writeAccess)
		return false;

	if(!getPaths(filename,newdir,fullpath))
		return false;

	// try to remove the file

	return _ftpParameters->fs->deleteFile(fullpath.c_str());
}


/**
 * Get the size of a file
 * @param filename
 * @param[out] fileSize The size in bytes.
 * @return true if it worked
 */

inline bool MyFtpServerConnection::fileSize(const char *filename,uint32_t& fileSize) {

	std::string fullpath,newdir;
	scoped_ptr<FileInformation> finfo;

	// get the full path

	if(!getPaths(filename,newdir,fullpath))
		return false;

	// get the file/directory information

	if(!_ftpParameters->fs->getFileInformation(fullpath.c_str(),finfo.address()))
		return false;

	// return the size

	fileSize=finfo->getLength();
	return true;
}


/**
 * Given a user's input file/dir, calc the full relative subdir and the full server path
 * @param dir The user's input dir
 * @param subdir The full relative subdir
 * @param fullpath The full server path
 * @return true if all OK
 */

inline bool MyFtpServerConnection::getPaths(const char *dir,std::string& subdir,std::string& fullpath) {

	// get the full path

	if(dir[0]=='/')
		subdir=dir;
	else
		subdir=appendPath(_cwd,dir);

	// remove any path navigation stuff

	if(!normalisePath(subdir))
		return false;

	// create the full path from the root and the normalised cwd

	fullpath=_ftpParameters->rootDirectory+'/'+subdir;
	return true;
}
