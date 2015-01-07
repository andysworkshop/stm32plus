/*
 * Copyright (c) 2012 Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Linaro Limited nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 */

//#include <string.h>
#include "config/debug.h"

namespace stm32plus{

    SemiHosting::Out_ SemiHosting::out;

int SemiHosting::open(const char *filename, int mode)
{
	struct {
		char const *filename;
		int mode;
		int filename_length;
	} args;

	args.filename = filename;
	args.mode = mode;
	args.filename_length = strlen(filename);

	return __semi_call(SYS_OPEN, (char*)&args);
}

int SemiHosting::close(int fd)
{
	return __semi_call(SYS_CLOSE, (char*)&fd);
}

int SemiHosting::puts(const char *s)
{
	return __semi_call(SYS_WRITE0, s);
}

int SemiHosting::write(int fd, char *buffer, int length)
{
	struct {
		int fd;
		char *buffer;
		int length;
	} args;

	args.fd = fd;
	args.buffer = buffer;
	args.length = length;

	return __semi_call(SYS_WRITE, (char*)&args);
}

int SemiHosting::read(int fd, char *buffer, int length)
{
	struct {
		int fd;
		char *buffer;
		int length;
	} args;

	args.fd = fd;
	args.buffer = buffer;
	args.length = length;

	return __semi_call(SYS_READ, (char*)&args);
}

int SemiHosting::flen(int fd)
{
	return __semi_call(SYS_FLEN, (char*)&fd);
}

int SemiHosting::get_cmdline(char *buffer, int size, int *length)
{
	int result;
	struct {
		char *buffer;
		int size;
	} args;

	args.buffer = buffer;
	args.size = size;

	result = __semi_call(SYS_GET_CMDLINE, (char *)&args);
	if(result)
		return result;

	if(length)
		*length = args.size;

	return 0;
}

int SemiHosting::reportexc(int reason)
{
	return __semi_call(SYS_REPORTEXC, (char *)reason);
}

void SemiHosting::exit(void)
{
	reportexc(REPORTEXC_REASON_APP_EXIT);
	while(1); /* should not be reached */
}

void SemiHosting::fatal(const char *message)
{
	puts(message);
	exit();
}

int SemiHosting::load_file(void **dest, unsigned *size, const char *filename)
{
	int result = -1;	/* fail by default */
	int fd = -1;
	int filesize;

	fd = open(filename, OPEN_RDONLY);
	if(fd == -1) {
		puts("Cannot open file: ");
		goto out;
	}

	filesize = flen(fd);
	if(filesize == -1) {
		puts("Cannot get file size for: ");
		goto out;
	}

	if(read(fd, (char*)*dest, filesize)) {
		puts("Could not read: ");
		goto out;
	}

	result = 0;	/* success */
	*dest = (char *)*dest + filesize;

out:
	if(fd != -1)
		close(fd);

	if(result) {	/* print context for the error message */
		puts(filename);
		puts("\n");
	} else
		if(size)
			*size = filesize;

	return result;
}

}
