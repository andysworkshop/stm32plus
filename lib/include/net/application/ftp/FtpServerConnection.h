/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * FTP server connection class. Template parameters are used to access the actual implementation
     * for serving files and the TCP implementation for maintaining data connections
     */

    template<class TImpl,class TTcp>
    class FtpServerConnection : public FtpServerConnectionBase {

      protected:
        TTcp *_tcpImpl;                   ///< TCP implementation - need this to get client data server

      protected:
        void onAccept(TcpAcceptEvent& event);
        void onTick(NetworkIntervalTickData& nitd);

        bool flushCommandStreams();
        bool executeCommand();
        void invalidSyntax();
        void addResponseString(const char *str);
        bool assertParameter(const char *str);
        bool assertLoggedIn();
        void directoryListing(bool (TImpl::*listingFunction)());
        void saveCommand(const char *param,bool append);

        void userCommand(const char *param);
        void passCommand(const char *param);
        void systCommand();
        void cwdCommand(const char *param);
        void pwdCommand();
        void mkdCommand(const char *param);
        void rmdCommand(const char *param);
        void pasvCommand();
        void nlstCommand();
        void helpCommand();
        void noopCommand();
        void listCommand();
        void aborCommand();
        void retrCommand(const char *param);
        void restCommand(const char *param);
        void typeCommand(const char *param);
        void appeCommand(const char *param);
        void storCommand(const char *param);
        void deleCommand(const char *param);
        void sizeCommand(const char *param);

      public:
        FtpServerConnection(const Parameters& params,TTcp* tcpImpl);

        bool handleRead();              ///< implementation requirement from the TcpConnectionArray
        bool handleWrite();             ///< implementation requirement from the TcpConnectionArray
        bool handleCallback();          ///< implementation requirement from the TcpConnectionArray
    };


    /**
     * Constructor
     * @param params reference to the parameters class
     * tcpImpl pointer to the network stack's TCP implementation
     */

    template<class TImpl,class TTcp>
    inline FtpServerConnection<TImpl,TTcp>::FtpServerConnection(const Parameters& params,TTcp *tcpImpl)
      : FtpServerConnectionBase(params),
        _tcpImpl(tcpImpl) {

      // start the last active timer

      updateLastActiveTime();
    }


    /**
     * Some data is ready for reading
     * @return true always: we don't want to ever abandon the client's wait() call
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::handleRead() {

      if(!_commandReceiver.add(*this)) {

        // error reading data from the client. don't return false as that would stop the whole server.
        // abort the connection

        delete this;
        return true;
      }

      // are we ready to run ?

      if(!_commandReceiver.ready())
        return true;

      // update the last active time

      updateLastActiveTime();

      // run the command and disconnect if it fails badly

      if(!executeCommand()) {

        flushCommandStreams();

        // close the connection

        delete this;
        return true;
      }

      // reset the command reader for the next one

      _commandReceiver.reset();
      return true;
    }


    /**
     * Write is possible, handle it
     * @return true always: we don't want to ever abandon the client's wait() call
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::handleWrite() {

      uint32_t actuallySent;

      // if we've just started, write the greeting

      if(_authenticationState==FtpServerAuthenticationState::STARTING) {

        if(!static_cast<TImpl *>(this)->writeGreeting()) {
          delete this;
          return true;
        }

        // advance to unauthenticated

        _authenticationState=FtpServerAuthenticationState::UNAUTHENTICATED;
        return true;
      }

      // write pending data to this connection

      if(!_outputStreams.writeDataToConnection(actuallySent))
        delete this;

      if(actuallySent)
        updateLastActiveTime();

      return true;
    }


    /**
     * We'll use the callback feature of the TCP connection array to service the data connection
     * @return true (no close)
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::handleCallback() {

      // check the pointer

      if(_dataConnection) {

        if(_dataConnection->getDirection()==FtpServerDataConnection::Direction::DOWNLOAD) {

          // writing data to the data connection

          if(_dataConnection->getConnectionState().state==TcpState::ESTABLISHED) {

            if(_dataConnection->handleWrite()) {

              // has it finished?

              if(_dataConnection->finished()) {

                // it has finished, close the port, reset the send counter and send a message

                freeDataConnection();
                _sendStartPosition=0;
                addResponseString("226 Closing data connection.");
              }
            }
          }
          else {

            // write failed, send a message and close the data connection

            addResponseString("426 Connection closed; transfer aborted.");
            freeDataConnection();
          }
        }
        else if(_dataConnection->getDirection()==FtpServerDataConnection::Direction::UPLOAD) {

          // reading data from the data connection

          if(_dataConnection->handleRead()) {

            // has it finished?

            if(_dataConnection->finished()) {

              // it has finished, close the port, reset the send counter and send a message

              freeDataConnection();
              addResponseString("226 Closing data connection.");
            }
          }
          else {

            // read failed, send a message and close the data connection

            addResponseString("426 Connection closed; transfer aborted.");
            freeDataConnection();
          }
        }
      }

      // finally check for an idle timeout

      if(static_cast<TImpl *>(this)->hasTimedOut(MillisecondTimer::millis()-_lastActiveTime))
        delete this;

      return true;
    }


    /**
     * Flush the command streams
     * @return false if there was a fail
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::flushCommandStreams() {

      uint32_t actuallySent;

      // flush the streams

      while(!_outputStreams.completed()) {
        if(_outputStreams.canWriteToConnection() && !_outputStreams.writeDataToConnection(actuallySent))
          return false;
      }

      return true;
    }


    /**
     * Execute the command
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::executeCommand() {

      const char *cmd,*param;
      int size;
      std::string str;

      // the client can embed telnet control characters anywhere in the command. Maybe 40 years ago
      // that seemed like a good idea. Today it's just data corruption. Get rid of them.

      const std::string& rawCommand(_commandReceiver.str());
      str.reserve(rawCommand.length());

      for(auto it=rawCommand.begin();it!=rawCommand.end();it++)
        if((*it & 0x80)==0)
          str+=*it;

      // the spec says ONE space is allowed between the cmd and
      // the optional parameter

      cmd=str.c_str();

      if((param=strchr(cmd,' '))!=nullptr) {
        param++;
        size=param-cmd-1;
      }
      else
        size=_commandReceiver.str().length();

      // sometimes we get telnet

      if(!strncasecmp(cmd,"USER",size))
        userCommand(param);
      else if(!strncasecmp(cmd,"PASS",size))
        passCommand(param);
      else if(!strncasecmp(cmd,"SYST",size))
        systCommand();
      else if(!strncasecmp(cmd,"TYPE",size))
        typeCommand(param);
      else if(!strncasecmp(cmd,"CWD",size) || !strncasecmp(cmd,"XCWD",size))
        cwdCommand(param);
      else if(!strncasecmp(cmd,"PWD",size))
        pwdCommand();
      else if(!strncasecmp(cmd,"PASV",size))
        pasvCommand();
      else if(!strncasecmp(cmd,"NLST",size))
        nlstCommand();
      else if(!strncasecmp(cmd,"LIST",size))
        listCommand();
      else if(!strncasecmp(cmd,"REST",size))
        restCommand(param);
      else if(!strncasecmp(cmd,"RETR",size))
        retrCommand(param);
      else if(!strncasecmp(cmd,"ABOR",size))
        aborCommand();
      else if(!strncasecmp(cmd,"HELP",size))
        helpCommand();
      else if(!strncasecmp(cmd,"NOOP",size))
        noopCommand();
      else if(!strncasecmp(cmd,"MKD",size) || !strncasecmp(cmd,"XMKD",size))
        mkdCommand(param);
      else if(!strncasecmp(cmd,"RMD",size) || !strncasecmp(cmd,"XRMD",size))
        rmdCommand(param);
      else if(!strncasecmp(cmd,"STOR",size))
        storCommand(param);
      else if(!strncasecmp(cmd,"APPE",size))
        appeCommand(param);
      else if(!strncasecmp(cmd,"DELE",size))
        deleCommand(param);
      else if(!strncasecmp(cmd,"SIZE",size))
        sizeCommand(param);
      else if(!strncasecmp(cmd,"QUIT",size)) {

        // ask the subclass tow write its goodbye message

        static_cast<TImpl *>(this)->writeGoodbye();

        // return false to close the connection

        return false;
      }
      else
        addResponseString("500 Invalid command.");

      return true;
    }


    /**
     * Invalid syntax: return error message
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::invalidSyntax() {
      addResponseString("501 Invalid syntax.");
    }


    /**
     * Add a new constant string to the output
     * @param str constant string to add excluding "\r\n"
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::addResponseString(const char *str) {

      std::string *newstr;

      newstr=new std::string(str);
      (*newstr)+="\r\n";

      _outputStreams.addStream(
        new StlStringInputStream(
            newstr,true),
            true
          );
    }


    /**
     * Validate that the parameter is not null
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::assertParameter(const char *str) {

      if(str==nullptr || *str=='\0') {
        addResponseString("501 Parameter missing.");
        return false;
      }
      return true;
    }


    /**
     * Validate that the user is logged in
     */

    template<class TImpl,class TTcp>
    inline bool FtpServerConnection<TImpl,TTcp>::assertLoggedIn() {

      if(_authenticationState==FtpServerAuthenticationState::AUTHENTICATED)
        return true;

      addResponseString("530 Not logged in.");
      return false;
    }


    /**
     * Handle the USER command
     * @return true if it worked
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::userCommand(const char *param) {

      if(!assertParameter(param))
        return;

      // attempt to switch user clears auth state

      _authenticationState=FtpServerAuthenticationState::UNAUTHENTICATED;

      if(!strcasecmp(param,"anonymous")) {

        // if anonymous access is not permitted, reject

        if(!static_cast<TImpl *>(this)->isAnonymousPermitted()) {
          addResponseString("530 Anonymous access forbidden");
          _user.clear();
        }
        else {
          addResponseString("331 Anonymous access granted, enter email address as password");
          _user=param;
        }
      }
      else {
        _user=param;
        addResponseString("331 Enter your password");
      }
    }


    /**
     * Handle the PASS command
     * @return true if it worked
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::passCommand(const char *param) {

      if(!assertParameter(param))
        return;

      if(_user.empty())
        addResponseString("503 The USER command is required before PASS");
      else {

        if(_user=="anonymous" && static_cast<TImpl *>(this)->isAnonymousPermitted()) {

          addResponseString("230 Anonymous access granted");
          _authenticationState=FtpServerAuthenticationState::AUTHENTICATED;
        }
        else {

          // validate the user

          if(static_cast<TImpl *>(this)->loginUser(_user.c_str(),param)) {
            _authenticationState=FtpServerAuthenticationState::AUTHENTICATED;
            addResponseString("230 Access granted");
          }
          else {
            MillisecondTimer::delay(3000);
            addResponseString("530 Access denied");
            _user.clear();
          }
        }
      }
    }


    /**
     * SYST command. Due to abuse of this command over the years we return
     * a meaningless string
     * @return true
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::systCommand() {
      addResponseString("215 UNIX Type: L8");
    }


    /**
     * TYPE command. Set the transfer type. We only support binary.
     * @return true
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::typeCommand(const char *param) {

      if(!assertParameter(param) || !assertLoggedIn())
        return;

      if((param[0] | 32)=='i')
        addResponseString("200 Type set to binary");
      else if((param[0] | 32)=='a')
        addResponseString("200 Type set to ascii");
      else
        addResponseString("500 Invalid type - this server only supports binary");
    }


    /**
     * CWD command. Set the working directory
     * @return true
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::cwdCommand(const char *param) {

      // the pathname is the parameter

      if(!assertParameter(param) || !assertLoggedIn())
        return;

      // call the subclass to store the pathname

      if(!static_cast<TImpl *>(this)->setCwd(param)) {
        addResponseString("550 Failed to change directory.");
        return;
      }

      addResponseString("250 OK");
    }


    /**
     * PWD command. Show the working directory
     * @return true if logged in
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::pwdCommand() {

      if(!assertLoggedIn())
        return;

      // send the response

      std::string cwd(static_cast<TImpl *>(this)->getCwd());

      addResponseString(("257 \""+cwd+"\"").c_str());
    }


    /**
     * PASV command. Respond with an IP address and port number
     * @return true if logged in
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::pasvCommand() {

      char buffer[20],*ptr;

      if(!assertLoggedIn())
        return;

      // if a data connection server is already active then close it. don't do this elsewhere
      // as the TCP server class contains the TCP connection parameters that all connections
      // reference.

      freeDataConnectionServer();
      freeDataConnection();

      // get an ephemeral port

      if(!_tcpImpl->ip_acquireEphemeralPort(_dataConnectionPortNumber)) {
        addResponseString("500 No more ephemeral ports are available");
        return;
      }

      // create the data connection server

      if(!_tcpImpl->template tcpCreateServer<FtpServerDataConnection,FtpServerConnectionBase>(
              _dataConnectionPortNumber,
              _dataConnectionServer,
              this)) {

        // didn't work, free the ephemeral port

        _tcpImpl->ip_releaseEphemeralPort(_dataConnectionPortNumber);
        addResponseString("500 No more data connection servers are available");
        return;
      }

      // subscribe to accept events from the data connection server

      _dataConnectionServer->TcpAcceptEventSender.insertSubscriber(
          TcpAcceptEventSourceSlot::bind(this,&FtpServerConnection<TImpl,TTcp>::onAccept));

      // start the server

      _dataConnectionServer->start();

      // get the IP address separated by commas

      _tcpImpl->getIpAddress().toString(buffer);
      for(ptr=buffer;*ptr;ptr++)
        if(*ptr=='.')
          *ptr=',';

      std::string response("227 Entering Passive Mode (");
      response+=buffer;

      // add the port number

      response+=',';
      StringUtil::modp_uitoa10(_dataConnectionPortNumber >> 8,buffer);
      response+=buffer;

      response+=',';
      StringUtil::modp_uitoa10(_dataConnectionPortNumber & 0xff,buffer);
      response+=buffer;

      response+=").";

      // send the response

      addResponseString(response.c_str());
    }


    /**
     * A connection-accept event has been received, auto-add it to the array
     * @param event The accept event
     */

    template<class TImpl,class TTcp>
    __attribute__((noinline)) inline void FtpServerConnection<TImpl,TTcp>::onAccept(TcpAcceptEvent& event) {

      // the accept must be from the server that we are subscribed to

      if(&event.server==_dataConnectionServer) {

        // ensure there's only one

        freeDataConnection();

        // store the connection and mark it as being accepted

        _dataConnection=static_cast<FtpServerDataConnection *>(event.connection);
        event.accepted=true;
      }
    }


    /**
     * STOR: write a file to the server
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::storCommand(const char *param) {
      saveCommand(param,false);
    }


    /**
     * APPE: append a file to one on the server
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::appeCommand(const char *param) {
      saveCommand(param,true);
    }


    /**
     * Save a file to the server, with append option
     * @param param The parameter to save
     * @param append true to append, false to create/replace as needed
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::saveCommand(const char *param,bool append) {

      // must be logged in

      if(!assertParameter(param) || !assertLoggedIn())
        return;

      // must have a data connection

      if(_dataConnection==nullptr)
        addResponseString("425 Can't open data connection.");
      else {

        OutputStream *uploadStream;

        // this is an upload

        _dataConnection->setDirection(FtpServerDataConnection::Direction::UPLOAD);

        // send the marker

        if(!static_cast<TImpl *>(this)->upload(param,append,uploadStream)) {
          addResponseString("550 Unable to upload the file, sorry");
          freeDataConnection();
        }
        else {

          _dataConnection->setUploadStream(uploadStream);

          // ensure that the 150 mark gets out before the data connection finishes
          // which could happen in a small upload

          addResponseString("150 Upload command accepted");
          flushCommandStreams();
        }
      }
    }


    /**
     * NLST: simple directory listing
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::nlstCommand() {
      directoryListing(&TImpl::simpleListing);
    }


    /**
     * LIST: complex directory listing
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::listCommand() {
      directoryListing(&TImpl::complexListing);
    }


    /**
     * Generic directory lister (calls the fn ptr supplied by caller)
     * @param listingFunction The function to call that will complete the listing
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::directoryListing(bool (TImpl::*listingFunction)()) {

      // must be logged in

      if(!assertLoggedIn())
        return;

      // must have a data connection

      if(_dataConnection==nullptr)
        addResponseString("425 Can't open data connection.");
      else {

        // this is a download

        _dataConnection->setDirection(FtpServerDataConnection::Direction::DOWNLOAD);

        // do the listing

        if(!(static_cast<TImpl *>(this)->*listingFunction)())
          addResponseString("550 Unable to read directory (does it exist?)");
        else {

          // flush the data connection and close it

          addResponseString("150 File status okay; about to open data connection.");

          flushCommandStreams();

          if(_dataConnection->flush())
            addResponseString("226 Directory listing completed");
          else
            addResponseString("426 Directory listing failed");
        }

        freeDataConnection();
      }
    }


    /**
     * REST: reset the start position
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::restCommand(const char *param) {

      if(assertParameter(param) && assertLoggedIn()) {

        _sendStartPosition=atoi(param);
        addResponseString("350 Requested file action pending further information.");
      }
    }


    /**
     * RETR: download a file
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::retrCommand(const char *param) {

      if(!assertParameter(param) || !assertLoggedIn())
        return;

      // must have a data connection

      if(_dataConnection==nullptr) {
        addResponseString("425 Can't open data connection.");
        freeDataConnection();
      }
      else {

        // this is a download

        _dataConnection->setDirection(FtpServerDataConnection::Direction::DOWNLOAD);

        if(!static_cast<TImpl *>(this)->download(param))
          addResponseString("550 Unable to download the file (does it exist?)");
        else {

          addResponseString("150 Download command accepted");

          // ensure that the 150 mark gets out before the data connection finishes
          // which could happen in a small download

          flushCommandStreams();
        }
      }
    }


    /**
     * NOOP: simple response
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::noopCommand() {
      addResponseString("200 OK");
    }


    /**
     * MKD: create a directory
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::mkdCommand(const char *param) {

      std::string newdir;

      if(assertParameter(param) && assertLoggedIn()) {

        if(static_cast<TImpl *>(this)->createDirectory(param,newdir)) {
          newdir="257 \""+newdir+"\" directory created";
          addResponseString(newdir.c_str());
        }
        else
          addResponseString("550 Failed to create directory");
      }
    }


    /**
     * RMD: remove a directory
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::rmdCommand(const char *param) {

      if(assertParameter(param) && assertLoggedIn()) {

        if(static_cast<TImpl *>(this)->removeDirectory(param)) {
          addResponseString("250 Directory removed");
        }
        else
          addResponseString("550 Failed to remove directory");
      }
    }


    /**
     * DELE: remove a file
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::deleCommand(const char *param) {

      if(assertParameter(param) && assertLoggedIn()) {

        if(static_cast<TImpl *>(this)->removeFile(param)) {
          addResponseString("250 File removed");
        }
        else
          addResponseString("550 Failed to remove file");
      }
    }


    /**
     * SIZE: return the transfer size of a file
     * @param param The user parameter to the command
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::sizeCommand(const char *param) {

      uint32_t fileSize;
      char buffer[20];

      if(assertParameter(param) && assertLoggedIn()) {

        if(static_cast<TImpl *>(this)->fileSize(param,fileSize)) {
          strcpy(buffer,"213 ");
          StringUtil::modp_uitoa10(fileSize,buffer+4);
          addResponseString(buffer);
        }
        else
          addResponseString("550 Size not available");
      }
    }


    /**
     * HELP: show a one liner
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::helpCommand() {

      char buffer[6];
      std::string str;

      // show the build number

      buffer[0]='0'+((STM32PLUS_BUILD >> 16) & 0xff);
      buffer[1]='.';
      buffer[2]='0'+((STM32PLUS_BUILD >> 8) & 0xff);
      buffer[3]='.';
      buffer[4]='0'+(STM32PLUS_BUILD & 0xff);
      buffer[5]='\0';

      str="214 stm32plus ";
      str+=buffer;
      str+=" ftp server ready and waiting";

      addResponseString(str.c_str());
    }


    /**
     * ABOR: abort the data transfer
     */

    template<class TImpl,class TTcp>
    inline void FtpServerConnection<TImpl,TTcp>::aborCommand() {

      // must be logged in

      if(assertLoggedIn()) {

        // abort if in progress

        if(_dataConnection!=nullptr) {
          freeDataConnection();
          addResponseString("426 Download aborted");
        }

        addResponseString("226 Abort successfully processed");
      }
    }
  }
}
