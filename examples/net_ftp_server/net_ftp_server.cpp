/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net_ftp.h"
#include "config/timing.h"
#include "config/sdcard.h"
#include "config/hash.h"
#include "config/filesystem.h"
#include "FtpServer.h"
#include "MyFtpServerConnection.h"


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * This demo brings together a number of the stm32plus components, namely
 * the network stack, the RTC, the SD card and the FAT16/32 filesystem
 * to build a simple ftp server that listens on port 21.
 *
 * The server supports authenticated and (optionally) anonymous connections.
 * It requires you to create a top-level directory on your SD card called
 * "ftp" and that "ftp" directory must contain a text file called "server.ini".
 * An example "server.ini" file is included in the same directory as this
 * example. Every option is explained in detail in the example server.ini,
 * please do read it.
 *
 * Only passive mode is supported by this server. I have tested this server
 * against cygwin/linux command line clients as well as IE10, Firefox 13 and
 * Chrome 28. The Windows 7 command line ftp client is not supported due to
 * its lack of passive mode support.
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                 |
 *              +------+---------------------+
 * TRANSPORT:   | Tcp | Udp | Icmp           |
 *              +-----++---------------------+
 * NETWORK      | DefaultIp | Arp            |
 *              +-----+----------------+-----+
 * DATALINK:    | DefaultRmiiInterface | Mac |
 *              +----------------------+-----+
 * PHYSICAL:    | DP83848C                   |
 *              +----------------------------+
 *
 * This example is only compatible with the F4 because it requires the SDIO
 * peripheral to communicate with the SD card.
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class NetFtpServerTest {

  public:

    /*
     * The network stack, sdio and filesystem objects that we'll need for this demo
     */

    MyNetworkStack *_net;
    BlockDevice *_sdcard;
    FileSystem *_fs;
    RtcTimeProvider *_timeProvider;


    /*
     * Run the test
     */

    void run() {

      FtpParameters ftpParams;

      // declare the RTC that that stack requires. it's used for cache timeouts, DHCP lease expiry
      // and such like so it does not have to be calibrated for accuracy. A few seconds here or there
      // over a 24 hour period isn't going to make any difference. Start it ticking at zero which is
      // some way back in 2000 but that doesn't matter to us

      Rtc<RtcLsiClockFeature<Rtc32kHzLsiFrequencyProvider>,RtcSecondInterruptFeature> rtc;
      rtc.setTick(0);

      // create the RTC time provider for the file system. if writes are made to the card then
      // this provider will be used to timestamp them.

      _timeProvider=new RtcTimeProvider(rtc);

      // declare the SD card with a 3 block (1.5kbyte) read cache
      // and check for error. the card must be inserted at this point

      _sdcard=new CachedBlockDevice(*new SdioDmaSdCard,3);

      if(errorProvider.hasError())
        error();

      // initialise a file system for the card. FAT16 and FAT32 are supported. the card must
      // already be formatted.

      if(!FileSystem::getInstance(*_sdcard,*_timeProvider,_fs))
        error();

      // read the server parameters file

      readServerParameters(ftpParams);

      // declare an instance of the network stack

      MyNetworkStack::Parameters params;
      _net=new MyNetworkStack;

      // the stack requires the RTC

      params.base_rtc=&rtc;

      // It's nice to give the DHCP client a host name because then it'll show up in DHCP
      // 'active leases' page. In a home router this is often called 'attached devices'

      params.dhcp_hostname="stm32plus";

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      // create an FTP server on port 21.
      // Here we take advantage of the second template parameter to the TcpServer template to
      // pass in a user-defined type to the constructor of MyFtpConnection. We use it to pass
      // in a pointer to some parameters we need to pass in, such as the TCP implementation
      // and the filesystem object

      ftpParams.fs=_fs;
      ftpParams.tcpImpl=_net;

      TcpServer<MyFtpServerConnection,FtpParameters> *ftpServer;

      if(!_net->tcpCreateServer(21,ftpServer,&ftpParams))
        error();

      // create an array to hold the active connections and configure it to
      // automatically receive connections as they arrive. It will also
      // automatically remove connections as they are closed.

      TcpConnectionArray<MyFtpServerConnection> connections(*ftpServer);

      // now all the plumbing is in place, open up the server to start
      // accepting connection requests

      ftpServer->start();

      // loop forever servicing connections via their handleXXX() methods
      // handleRead()     : data is ready to read
      // handleWrite()    : connection can accept new data
      // handleClosed()   : connection was closed (either end)
      // handleCallback() : round-robin callback to do whatever you want with (we use it to service the data connection)

      connections.wait(TcpWaitState::READ | TcpWaitState::WRITE | TcpWaitState::CLOSED | TcpWaitState::CALLBACK,0);
    }


    /*
     * Read the FTP server parameters. the syntax is a simple key = value style
     * See comments in the demo ftpserver.ini (in this directory) for help.
     * @return false if it fails
     */

    bool readServerParameters(FtpParameters& params) {

      scoped_ptr<File> file;
      char buffer[200];
      uint16_t pos;
      uint32_t actuallyRead;

      // open the parameters file

      if(!_fs->openFile("/ftp/server.ini",file.address()))
        return false;

      FileInputStream fis(*file);

      for(;;) {

        // read a line

        for(pos=0;pos<sizeof(buffer)-1;pos++) {

          // check for error

          if(!fis.read(&buffer[pos],1,actuallyRead))
            return false;

          // check for EOF

          if(actuallyRead==0)
            return true;

          // check for EOL

          if(buffer[pos]=='\n')
            break;
        }

        // terminate the line

        buffer[pos]='\0';

        std::string line(buffer);
        std::string key,value;

        if(line.length()==0 || line[0]=='#' || !getKeyValue(line,key,value))
          continue;

        if(key=="greeting")
          params.serverGreeting+=value+"\r\n";
        else if(key=="goodbye")
          params.serverGoodbye+=value+"\r\n";
        else if(key=="anonymous") {
          params.anonymousPermitted=value=="on";
          params.users.push_back(FtpUser::createAnonymous());
        }
        else if(key=="root")
          params.rootDirectory=value;
        else if(key=="user")
          parseUser(params,value);
        else if(key=="idletimeout")
          params.idleTimeout=atoi(value.c_str());
      }
    }


    /**
     * Parse a user specification: user=name:password-salt:password-hash:r/w
     * See comments in ftpserver.ini for how to generate passwords with sha1sum
     * @param params The FTP parameters
     * @param userspec The user specification
     */

    void parseUser(FtpParameters& params,const std::string& userspec) {

      std::vector<std::string> words;
      FtpUser user;

      // syntax is (e.g.) user=andy:lskdjfoe:86f4d33bf6616c7df658367c69fa43712b072c88:w

      StdStringUtil::tokenise(userspec,":",words);

      if(words.size()==4) {

        user.name=words[0];
        user.passwordSalt=words[1];
        user.passwordHash=words[2];
        user.writeAccess=words[3]=="w";

        params.users.push_back(user);
      }
    }


    /**
     * Get a key and a value from the line we read in
     */

    bool getKeyValue(const std::string& line,std::string& key,std::string& value) {

      std::string::size_type pos;

      if((pos=line.find('='))==std::string::npos)
        return false;

      key=line.substr(0,pos);
      StdStringUtil::trim(key);

      value=line.substr(pos+1);
      StdStringUtil::trim(value);

      return true;
    }


    void error() {
      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  // interrupts
  Nvic::initialise();

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  NetFtpServerTest test;
  test.run();

  // not reached
  return 0;
}
