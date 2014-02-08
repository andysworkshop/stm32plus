/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /*
     * Constructor
     */

    FatFileSystem::FatFileSystem(BlockDevice& blockDevice,const TimeProvider& timeProvider,const fat::BootSector& bootSector,uint32_t firstSectorIndex,uint32_t countOfClusters) :
      FileSystem(blockDevice,timeProvider,firstSectorIndex) {

      _countOfClusters=countOfClusters;
      _bootSector=bootSector; // struct copy
      _fatFirstSector=_bootSector.BPB_RsvdSecCnt; // sector index of the FAT
      _sectorsPerBlock=blockDevice.getBlockSizeInBytes() / _bootSector.BPB_BytsPerSec;
    }

    /**
     * Virtual destructor, does nothing.
     */

    FatFileSystem::~FatFileSystem() {
    }

    /**
     * Get the correct FAT implementation for this block device.
     *
     * @param[in] blockDevice The block device that holds the file system.
     * @param[in] timeProvider A provider of the current time that will be used by the methods that create and modify data to
     * update the directory entry with write times.
     * @param[out] newFileSystem Reference to a caller supplied pointer that will be filled in with the appropriate FatFileSystem instance.
     * The caller owns this pointer and must delete it when finished.
     * @return false if it fails, error provider will be filled in.
     */

    bool FatFileSystem::getInstance(BlockDevice& blockDevice,const TimeProvider& timeProvider,FatFileSystem*& newFileSystem) {

      ByteMemblock bootSectorBytes(blockDevice.getBlockSizeInBytes());
      fat::BootSector bs;
      uint32_t rootDirSectors,fatSize,totalSectors,dataSectors,countOfClusters,firstSectorIndex;

      // read block zero

      if(!blockDevice.readBlock(bootSectorBytes,0))
        return false;

      // check the signature bytes

      if(bootSectorBytes[510] != 0x55 || bootSectorBytes[511] != 0xaa)
        return false;

      // copy the boot sector - the endian-ness matches the STM32

      memcpy(&bs,bootSectorBytes,sizeof(bs));

      // is this REALLY a boot sector and not an MBR?

      if((bs.BS_jmpBoot[0]!=0xe9 && bs.BS_jmpBoot[0]!=0xeb) || bs.BPB_BytsPerSec!=512) {

        Mbr *mbr=reinterpret_cast<Mbr *>(bootSectorBytes.getData());

        firstSectorIndex=mbr->partitions[0].lbaFirstSector;
        if(!blockDevice.readBlock(bootSectorBytes,firstSectorIndex))
          return false;

        if(bootSectorBytes[510] != 0x55 || bootSectorBytes[511] != 0xaa)
          return false;

        memcpy(&bs,bootSectorBytes,sizeof(bs));
      }
      else
        firstSectorIndex=0;

      // check that the sector size matches

      if(blockDevice.getBlockSizeInBytes() != bs.BPB_BytsPerSec)
        return false;

      // calculate the total clusters
      // note that on a FAT32 volume, the BPB_RootEntCnt value is always 0; so on a FAT32 volume, RootDirSectors is always 0

      rootDirSectors=((bs.BPB_RootEntCnt * 32) + (bs.BPB_BytsPerSec - 1)) / bs.BPB_BytsPerSec;

      // next, we determine the count of sectors in the data region of the volume:

      if(bs.BPB_FATSz16 != 0)
        fatSize=bs.BPB_FATSz16;
      else
        fatSize=bs.fat32.BPB_FATSz32;

      if(bs.BPB_TotSec16 != 0)
        totalSectors=bs.BPB_TotSec16;
      else
        totalSectors=bs.BPB_TotSec32;

      dataSectors=totalSectors - (bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs * fatSize) + rootDirSectors);

      // now we determine the count of clusters:

      countOfClusters=dataSectors / bs.BPB_SecPerClus;

      // now return an appropriate FS

      if(countOfClusters < 4085)
        return false; // FAT12 not supported
      else if(countOfClusters < 65525)
        newFileSystem=new Fat16FileSystem(blockDevice,timeProvider,bs,firstSectorIndex,countOfClusters);
      else
        newFileSystem=new Fat32FileSystem(blockDevice,timeProvider,bs,firstSectorIndex,countOfClusters);

      return true;
    }

    /**
     * Read a sector from a cluster in a file.
     * @param[in] clusterIndex The cluster index of the sector.
     * @param[in] sectorIndexInCluster The sector index in the cluster, with zero being the first sector in the cluster.
     * @param[in] buffer The buffer to receive the sector data. Must be large enough.
     * @return false if it fails.
     */

    bool FatFileSystem::readSectorFromCluster(uint32_t clusterIndex,uint32_t sectorIndexInCluster,void *buffer) {

      uint32_t sectorIndex;

      // convert the cluster number to a sector number

      sectorIndex=sectorIndexInCluster + clusterToSector(clusterIndex);

      // read it

      return readSector(sectorIndex,buffer);
    }

    /**
     * Write a sector to a cluster.
     * @param[in] clusterIndex The cluster index of the sector.
     * @param[in] sectorIndexInCluster The sector index in the cluster, with zero being the first sector in the cluster.
     * @param[in] buffer The buffer that holds the sector data to write.
     * @return false if it fails.
     */

    bool FatFileSystem::writeSectorToCluster(uint32_t clusterIndex,uint32_t sectorIndexInCluster,void *buffer) {

      uint32_t sectorIndex;

      // convert the cluster number to a sector number

      sectorIndex=sectorIndexInCluster + clusterToSector(clusterIndex);

      // write it

      return writeSector(sectorIndex,buffer);
    }

    /**
     * Read a fat entry for a cluster.
     * @param[in] clusterNumber The cluster number to read from.
     * @param[out] fatEntryForCluster The FAT entry for this cluster.
     * @return false if it fails
     */

    bool FatFileSystem::readFatEntry(uint32_t clusterNumber,uint32_t& fatEntryForCluster) {

      uint32_t sectorIndex,fatEntOffset,fatOffset;
      ByteMemblock sector(_bootSector.BPB_BytsPerSec);

      // get the byte offset into the fat of the cluster entry

      fatOffset=clusterNumber * getFatEntrySizeInBytes();

      // now get the sector index that holds the fat entry and the offset into that sector of the fat entry

      sectorIndex=_bootSector.BPB_RsvdSecCnt + (fatOffset / _bootSector.BPB_BytsPerSec);
      fatEntOffset=fatOffset % _bootSector.BPB_BytsPerSec;

      // read the sector

      if(!readSector(sectorIndex,sector))
        return false;

      // get the value from the fat

      fatEntryForCluster=getFatEntryFromMemory(static_cast<uint8_t *> (sector) + fatEntOffset);
      return true;
    }

    /**
     * Return a new directory iterator for the directory at the given path.
     *
     * @param[in] pathname The path to the directory to iterate over.
     * @param[out] newIterator A pointer reference that this class will create. The caller owns this pointer and must delete it when finished.
     */

    bool FatFileSystem::getDirectoryIterator(const char *pathname,DirectoryIterator*& newIterator) {

      FatDirectoryIterator *it;

      if(!FatDirectoryIterator::getInstance(*this,pathname,it))
        return false;

      newIterator=it;
      return true;
    }

    /**
     * Get file information for the given pathname.
     * @param[in] filename The pathname to the file.
     * @param[out] info Reference to a file information structure that this call will create and fill in. The caller owns this pointer
     * and must delete it when finished.
     * @return false if it fails.
     */

    bool FatFileSystem::getFileInformation(const char *filename,FileInformation*& info) {

      DirectoryEntryWithLocation dirent;

      TokenisedPathname tp(filename);
      if(!getDirectoryEntry(tp,dirent))
        return false;

      info=new FatFileInformation(filename,dirent);
      return true;
    }


    /**
     * Create a new directory.
     * @param[in] dirname The full pathname to the new directory.
     * @return false if it fails.
     */

    bool FatFileSystem::createDirectory(const char *dirname) {

      DirectoryEntryWithLocation dirent;
      DirectoryEntry dot,dotDot;
      ByteMemblock sector(getSectorSizeInBytes());
      uint8_t i;

      // create a file for this directory

      if(!createFile(dirname))
        return false;

      // get the dirent for the 'file' that we just created

      TokenisedPathname tp(dirname);
      if(!getDirectoryEntry(tp,dirent))
        return false;

      // open the file and write a single zero'd cluster

      FatFile file(*this,dirent);
      memset(sector,0,getSectorSizeInBytes());

      for(i=0;i < _bootSector.BPB_SecPerClus;i++)
        if(!file.write(sector,getSectorSizeInBytes()))
          return false;

      // copy back the dirent from the modified file

      dirent=file.getDirectoryEntryWithLocation();

      // create the "." and ".." entries

      memset(&dot,0,sizeof(dot));
      memset(&dotDot,0,sizeof(dotDot));

      memset(dot.sdir.DIR_Name,' ',sizeof(dot.sdir.DIR_Name));
      dot.sdir.DIR_Name[0]='.';
      dot.sdir.DIR_Attr=DirectoryEntry::ATTR_DIRECTORY;
      dot.sdir.DIR_FstClusLO=dirent.Dirent.sdir.DIR_FstClusLO;
      dot.sdir.DIR_FstClusHI=dirent.Dirent.sdir.DIR_FstClusHI;
      dirent.copyTimesTo(dot);

      // get the cluster number of the parent

      if(!getParentDirectoryFirstCluster(tp,&dotDot.sdir.DIR_FstClusLO,&dotDot.sdir.DIR_FstClusHI))
        return false;

      memset(dotDot.sdir.DIR_Name,' ',sizeof(dotDot.sdir.DIR_Name));
      dotDot.sdir.DIR_Name[0]='.';
      dotDot.sdir.DIR_Name[1]='.';
      dotDot.sdir.DIR_Attr=DirectoryEntry::ATTR_DIRECTORY;
      dirent.copyTimesTo(dotDot);

      // write the two special directories

      if(!file.seek(0,File::SeekStart))
        return false;

      if(!file.write(&dot,sizeof(dot)) || !file.write(&dotDot,sizeof(dotDot)))
        return false;

      // now change the 'file' to be a directory and we're complete

      dirent.Dirent.sdir.DIR_Attr=DirectoryEntry::ATTR_DIRECTORY;
      dirent.Dirent.sdir.DIR_FileSize=0;

      return writeDirectoryEntry(dirent);
    }

    /**
     * Create a new file. The new file will have zero length. Long filenames are supported.
     * @param[in] filename The full pathname of the new file.
     * @return false if it fails.
     */

    bool FatFileSystem::createFile(const char *filename) {

      FatDirectoryIterator *it;
      bool retval;

      // tokenise the path, must have a component

      TokenisedPathname tp(filename);
      if(tp.getNumTokens() == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_INVALID_PATHNAME);

      // cannot create a file that exists

      if(getDirectoryIteratorPointingToFile(tp,it)) {
        delete it;
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_FILE_EXISTS);
      }

      // limit the range so that we get an iterator on to the parent

      tp.resetRange();
      tp.setRange(0,tp.getNumTokens() - 2);

      if(!FatDirectoryIterator::getInstance(*this,tp,it))
        return false;

      // reset the range so that we can see the filename on the end

      tp.resetRange();

      // create the dirents for this new file

      LongNameDirentGenerator lndg(tp.last(),it->getDirectoryEntryIterator(),0,0);

      if(errorProvider.getLast() != 0) {
        delete it;
        return false;
      }

      // write the dirents to the owning directory

      retval=it->getDirectoryEntryIterator().writeDirents(lndg.getDirents(),lndg.getDirentCount());
      delete it;
      return retval;
    }

    /**
     * Open an existing file.
     * @param[in] filename The full pathname of the existing file.
     * @param[out] newFile A pointer to the File class that the caller can use to manipulate the file. The caller must
     * delete this pointer when finished.
     * @return false if it fails.
     */

    bool FatFileSystem::openFile(const char *filename,File*& newFile) {

      DirectoryEntryWithLocation dirent;

      // tokenise the path

      TokenisedPathname tp(filename);
      if(tp.getNumTokens() == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_INVALID_PATHNAME);

      // get the directory entry for this file

      if(!getDirectoryEntry(tp,dirent))
        return false;

      // create the file object

      newFile=new FatFile(*this,dirent);
      return true;
    }

    /**
     * Return true if directory has content (files or directories).
     * @param[in] dirname The full pathame of the directory to test.
     * @param[out] hasContent Set to true if this directory has content, false if not.
     * @return false if it fails.
     */

    bool FatFileSystem::directoryHasContent(const char *dirname,bool& hasContent) {

      DirectoryIterator *it;

      // get an iterator on to the directory

      if(!getDirectoryIterator(dirname,it))
        return false;

      // preset for dropping off the end of the loop

      hasContent=false;

      while(it->next()) {

        // any entry other than the two specials means we have content

        if(!it->isCurrentDirectory() && !it->isParentDirectory()) {
          hasContent=true;
          break;
        }
      }

      delete it;
      return true;
    }

    /**
     * Delete a directory. The directory must be empty of all files and subdirectories.
     * @param[in] dirname_ The full pathname to the directory.
     * @return false if it fails.
     */

    bool FatFileSystem::deleteDirectory(const char *dirname_) {

      FatDirectoryIterator *it;
      bool retval,hasContent;

      // cannot have content

      if(!directoryHasContent(dirname_,hasContent))
        return false;

      if(hasContent)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_DIRECTORY_NOT_EMPTY);

      // tokenise the path. can't delete the root directory

      TokenisedPathname tp(dirname_);
      if(tp.getNumTokens() == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_INVALID_PATHNAME);

      // get the directory entry for the file

      if(!getDirectoryIteratorPointingToFile(tp,it))
        return false;

      // it must be a file

      DirectoryEntryWithLocation& dirent=it->getDirectoryEntryWithLocation();

      if(!dirent.isDirectory())
        retval=errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_NOT_A_DIRECTORY);
      else if(it->isParentDirectory() || it->isCurrentDirectory())
        retval=errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_INVALID_PATHNAME);
      else
        retval=fullyDelete(*it);

      delete it;
      return retval;
    }

    /**
     * Delete a file.
     * @param[in] filename The full pathname to the file that is to be deleted.
     * @return false if it fails.
     */

    bool FatFileSystem::deleteFile(const char *filename) {

      FatDirectoryIterator *it;
      bool retval;

      // tokenise the path

      TokenisedPathname tp(filename);
      if(tp.getNumTokens() == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_INVALID_PATHNAME);

      // get the directory entry for the file

      if(!getDirectoryIteratorPointingToFile(tp,it))
        return false;

      // it must be a file

      DirectoryEntryWithLocation& dirent=it->getDirectoryEntryWithLocation();
      if(!dirent.isFile())
        retval=errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_NOT_A_FILE);
      else
        retval=fullyDelete(*it);

      delete it;
      return retval;
    }

    /*
     * deallocate and delete
     */

    bool FatFileSystem::fullyDelete(FatDirectoryIterator& it) {

      DirectoryEntry& dirent=it.getDirectoryEntryWithLocation().Dirent;
      uint32_t firstCluster;

      // deallocate the cluster chain

      firstCluster=static_cast<uint32_t> (dirent.sdir.DIR_FstClusHI) << 16 | dirent.sdir.DIR_FstClusLO;
      if(firstCluster != 0)
        deAllocateClusterChain(firstCluster);

      // free the dirents that made up this filename

      return deleteDirents(it);
    }

    /*
     * delete the set of dirents that make up this file
     */

    bool FatFileSystem::deleteDirents(FatDirectoryIterator& fdi) {

      uint32_t i;
      FilenameHandler& fh=fdi.getDirectoryEntryIterator().getFilenameHandler();
      ByteMemblock sector(getSectorSizeInBytes());
      DirectoryEntry *de;
      uint32_t lastSectorIndex,thisSectorIndex;

      lastSectorIndex=0;
      for(i=0;i < fh.getDirentCount();i++) {

        thisSectorIndex=fh.getSectorIndices()[i];

        // read the sector holding the dirent

        if(thisSectorIndex != lastSectorIndex) {

          if(lastSectorIndex != 0 && !writeSector(thisSectorIndex,sector))
            return false;

          if(!readSector(thisSectorIndex,sector))
            return false;

          lastSectorIndex=thisSectorIndex;
        }

        // get a pointer to the dirent and set the 'deleted' byte in the first char of the filename

        de=reinterpret_cast<DirectoryEntry *> (sector.getData() + fh.getSectorOffsetIndices()[i] * sizeof(DirectoryEntry));
        de->sdir.DIR_Name[0]=0xe5;
      }

      // write back the sector

      if(lastSectorIndex != 0 && !writeSector(lastSectorIndex,sector))
        return false;

      return true;
    }

    /**
     * De-allocate (free) a cluster chain. Failure may result in lost clusters.
     * @param[in] firstCluster The first cluster in the chain.
     * @return false if it fails.
     */

    bool FatFileSystem::deAllocateClusterChain(uint32_t firstCluster) {

      uint32_t clusterNumber;

      // de-allocate every cluster in the chain

      ClusterChainIterator cit(*this,firstCluster,ClusterChainIterator::extensionDontExtend);

      // free clusters by setting each one in the chain to zero one step behind the iterator

      clusterNumber=0;
      while(cit.next()) {

        // free the last cluster number

        if(clusterNumber != 0 && !writeFatEntry(clusterNumber,0))
          return false;

        clusterNumber=cit.current();
      }

      if(clusterNumber != 0 && !writeFatEntry(clusterNumber,0))
        return false;

      return true;
    }

    /*
     * Get the directory entry for a file
     */

    bool FatFileSystem::getDirectoryEntry(TokenisedPathname& pathTokens,DirectoryEntryWithLocation& dirent) {

      FatDirectoryIterator *parent;

      if(!getDirectoryIteratorPointingToFile(pathTokens,parent))
        return false;

      // get the directory entry from the iterator

      dirent=parent->getDirectoryEntryWithLocation(); // struct copy
      delete parent;

      return true;
    }

    /*
     * Get the directory iterator pointing to a file
     */

    bool FatFileSystem::getDirectoryIteratorPointingToFile(TokenisedPathname& pathTokens,FatDirectoryIterator *& parent) {

      // limit the range so that we get an iterator on to the parent

      pathTokens.setRange(0,pathTokens.getNumTokens() - 2);
      if(!FatDirectoryIterator::getInstance(*this,pathTokens,parent))
        return false;

      // reset the range so that we can see the filename on the end

      pathTokens.resetRange();

      // move the directory iterator to this filename

      if(!parent->moveTo(pathTokens.last())) {
        delete parent;
        parent=nullptr;
        return false;
      }

      return true;
    }

    /*
     * Get the first cluster number of the file's parent. Note that this returns zero if the root directory
     * is the parent even though there is a valid first cluster fo the root of FAT32 file systems
     */

    bool FatFileSystem::getParentDirectoryFirstCluster(TokenisedPathname& pathTokens,uint16_t* lo,uint16_t* hi) {

      FatDirectoryIterator *it;

      // limit the range so that we get an iterator on to the parent

      pathTokens.resetRange();
      pathTokens.setRange(0,pathTokens.getNumTokens() - 2);

      if(pathTokens.getNumTokens() == 0) {
        *lo=0; // the parent is the root directory
        *hi=0;
      } else {

        if(!FatDirectoryIterator::getInstance(*this,pathTokens,it))
          return false;

        // find "."

        if(!it->moveTo(".")) {
          delete it;
          return false;
        }

        DirectoryEntryWithLocation& dirent=it->getDirectoryEntryWithLocation();

        *lo=dirent.Dirent.sdir.DIR_FstClusLO;
        *hi=dirent.Dirent.sdir.DIR_FstClusHI;

        delete it;
      }

      return true;
    }

    /**
     * Get a reference to the boot sector
     * @return An internal reference to the filesystem boot sector.
     */

    const fat::BootSector& FatFileSystem::getBootSector() const {
      return _bootSector;
    }

    /**
     * Get the total cluster count.
     * @return The total cluster count.
     */

    uint32_t FatFileSystem::getCountOfClusters() const {
      return _countOfClusters;
    }

    /**
     * Convert cluster index to sector index.
     * @param[in] clusterNumber The cluster number to convert.
     * @return The sector index.
     */

    uint32_t FatFileSystem::clusterToSector(uint32_t clusterNumber) const {
      return _firstDataSector + ((clusterNumber - 2) * _bootSector.BPB_SecPerClus);
    }

    /**
     * Get the first sector of the root directory.
     * @return The first sector index.
     */

    uint32_t FatFileSystem::getRootDirectoryFirstSector() const {
      return _rootDirFirstSector;
    }

    /**
     * Get the size in bytes of a sector.
     * @return The sector size, in bytes.
     */

    uint32_t FatFileSystem::getSectorSizeInBytes() const {
      return _bootSector.BPB_BytsPerSec;
    }

    /**
     * Allocate a new cluster into the chain. This function will seek to the end of the cluster
     * chain and then allocate a new cluster and link it to the end of the chain.
     *
     * @param[in] anyClusterInChain Any cluster number in the chain.
     * @param[out] newCluster The newly allocated cluster number.
     * @return false if it fails.
     */

    bool FatFileSystem::allocateNewCluster(uint32_t anyClusterInChain,uint32_t& newCluster) {

      // find a free cluster

      if(!findFreeCluster(newCluster))
        return false;

      // step to the end of the cluster chain if this is not the first cluster in an empty file

      if(anyClusterInChain != 0) {

        ClusterChainIterator cit(*this,anyClusterInChain,ClusterChainIterator::extensionDontExtend);
        while(cit.next())
          ;

        // ensure reason for ending is that we hit the end

        if(!errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_ITERATOR,ClusterChainIterator::E_END_OF_ENTRIES))
          return false;

        // link the free cluster to the previous EOC

        if(!writeFatEntry(cit.current(),newCluster))
          return false;
      }

      // write EOC to free cluster

      return writeFatEntry(newCluster,getEndOfClusterChainMarker());
    }

    /**
     * Write an entry to both copies of the FAT. The assumption here is that both FAT entries are
     * identical, as they should be except in the case of recoverable corruption.
     * @param[in] fatEntryIndex The FAT index to write to.
     * @param[in] fatEntryContent The content of the entry to write.
     * @return false if it fails.
     */

    bool FatFileSystem::writeFatEntry(uint32_t fatEntryIndex,uint32_t fatEntryContent) {

      uint32_t sectorIndex,fatEntOffset,fatOffset;
      Memblock<uint8_t> sector(_bootSector.BPB_BytsPerSec);

      // get the byte offset into the fat of the cluster entry

      fatOffset=fatEntryIndex * getFatEntrySizeInBytes();

      // now get the sector index that holds the fat entry and the offset into that sector of the fat entry

      sectorIndex=_bootSector.BPB_RsvdSecCnt + (fatOffset / _bootSector.BPB_BytsPerSec);
      fatEntOffset=fatOffset % _bootSector.BPB_BytsPerSec;

      // read the sector from FAT #1

      if(!readSector(sectorIndex,sector))
        return false;

      // modify the value in the sector

      setFatEntryToMemory(sector + fatEntOffset,fatEntryContent);

      // write the sector back to FAT #1

      if(!writeSector(sectorIndex,sector))
        return false;

      // write the sector back to FAT #2 - big assumption here that FAT #1 and FAT#2 are identical

      sectorIndex+=getSectorsPerFat();
      return writeSector(sectorIndex,sector);
    }

    /**
     * Write a directory entry back to the device.
     * @param[in] dirent_ The directory entry to write back.
     * @return false if it fails.
     */

    bool FatFileSystem::writeDirectoryEntry(DirectoryEntryWithLocation& dirent_) {

      ByteMemblock sector(getSectorSizeInBytes());

      // read the sector containing the directory entry

      if(!readSector(dirent_.SectorNumber,sector))
        return false;

      // copy the dirent into the sector

      DirectoryEntry& dirent=dirent_.Dirent;
      memcpy(sector + sizeof(DirectoryEntry) * dirent_.IndexWithinSector,&dirent,sizeof(DirectoryEntry));

      // write back the sector

      return writeSector(dirent_.SectorNumber,sector);
    }

    /**
     * Find a free cluster. This implementation - being for an MCU - assumes that the FS is likely to
     * be on flash therefore the wear resistant implementation is used. Swap to the linear implementation
     * if this is not the case.
     *
     * @param[out] freeCluster The free cluster number.
     * @return false if it fails.
     */

    bool FatFileSystem::findFreeCluster(uint32_t& freeCluster) {

      WearResistFreeClusterFinder freeFinder(*this);
      return freeFinder.find(freeCluster);
    }

    /**
     * Get free space on the device in bytes.
     * Units will be clusters, the multiplier being sectorsPerCluster * bytesPerSector. So to get the free
     * space in bytes you need to multiply freeUnits_ by unitsMultiplier_. This allows for device sizes
     * greater than 4Gb.
     * @param[out] freeUnits The number of free units.
     * @param[out] unitsMultiplier What to multiply freeUnits_ by to get a byte size.
     * @return false if it fails.
     */

    bool FatFileSystem::getFreeSpace(uint32_t& freeUnits,uint32_t& unitsMultiplier) {

      uint32_t sectorIndex,entriesPerSector,i,count;
      ByteMemblock sector(getSectorSizeInBytes());
      uint8_t *ptr;

      // read each FAT sector

      freeUnits=0;
      count=0;

      entriesPerSector=getSectorSizeInBytes() / getFatEntrySizeInBytes();

      for(sectorIndex=_bootSector.BPB_RsvdSecCnt;sectorIndex < _bootSector.BPB_RsvdSecCnt + getSectorsPerFat() && count != _countOfClusters + 2;sectorIndex++) {

        // read the FAT sector

        if(!readSector(sectorIndex,sector))
          return false;

        // look for free entries in the FAT

        ptr=sector;
        for(i=0;i < entriesPerSector && count != _countOfClusters + 2;i++) {

          if(getFatEntryFromMemory(ptr) == 0)
            freeUnits++;

          ptr+=getFatEntrySizeInBytes();
          count++;
        }
      }

      // set the multiplier

      unitsMultiplier=static_cast<uint32_t> (_bootSector.BPB_SecPerClus) * getSectorSizeInBytes();
      return true;
    }
  }
}
