/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file to get access to all the filesytem functionality. At present this grants you access
 * to the FAT16 and FAT32 functionality.
 */

// filesystem depends on iterator, stream, device, timing, string

#include "config/iterator.h"
#include "config/stream.h"
#include "config/device.h"
#include "config/timing.h"
#include "config/string.h"

// includes for the feature

#include "filesystem/File.h"
#include "filesystem/FileInformation.h"
#include "filesystem/DirectoryIterator.h"
#include "filesystem/FileInputStream.h"
#include "filesystem/FileOutputStream.h"
#include "filesystem/FileReader.h"
#include "filesystem/OwnedFileInputStream.h"
#include "filesystem/OwnedFileOutputStream.h"
#include "filesystem/MbrPartition.h"
#include "filesystem/Mbr.h"
#include "filesystem/TokenisedPathname.h"
#include "filesystem/FileSystem.h"

#include "filesystem/fat/BootSector16.h"
#include "filesystem/fat/BootSector32.h"
#include "filesystem/fat/BootSector.h"


#include "filesystem/fat/LongDirectoryEntry.h"
#include "filesystem/fat/ShortDirectoryEntry.h"
#include "filesystem/fat/DirectoryEntry.h"
#include "filesystem/fat/DirectoryEntryWithLocation.h"
#include "filesystem/fat/FilenameHandler.h"
#include "filesystem/fat/DirectoryEntryIterator.h"

#include "filesystem/fat/ClusterChainIterator.h"
#include "filesystem/fat/FatFileInformation.h"
#include "filesystem/fat/FatIterator.h"
#include "filesystem/fat/FileSectorIterator.h"
#include "filesystem/fat/LongNameDirentGenerator.h"
#include "filesystem/fat/NormalDirectoryEntryIterator.h"

#include "filesystem/fat/FreeClusterFinder.h"
#include "filesystem/fat/IteratingFreeClusterFinder.h"
#include "filesystem/fat/LinearFreeClusterFinder.h"
#include "filesystem/fat/WearResistFreeClusterFinder.h"

#include "filesystem/fat/FatFile.h"
#include "filesystem/fat/FatFileSystem.h"
#include "filesystem/fat/FatFileSystemFormatter.h"

#include "filesystem/fat/Fat32FsInfo.h"
#include "filesystem/fat/Fat32FileSystem.h"
#include "filesystem/fat/Fat32FileSystemFormatter.h"

#include "filesystem/fat/Fat16FileSystem.h"
#include "filesystem/fat/Fat16FileSystemFormatter.h"
#include "filesystem/fat/Fat16RootDirectoryEntryIterator.h"
#include "filesystem/fat/FatDirectoryIterator.h"
