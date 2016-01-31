liblzg v1.0.6
=============

About
-----

liblzg is a minimal implementation of an LZ77 class compression library. The
main characteristic of the library is that the decoding routine is very simple,
fast and requires little memory.

In general, liblzg does not compress as well as zlib, for instance, and the
encoder is usually slower than comparable compression routines. On the other
hand the decoder is very simple and very fast - ideal for systems with tight
memory limits or processing capabilities.

For more information, see the Doxygen documentation (in the doc folder).


Compiling
---------

The easisest way to compile liblzg and its tools is to use the GCC toolchain,
and use the Makefile in the src folder. Just type 'make' in the src folder,
which should create a static link library (in the src/lib folder) and the
example tools (in the src/tools folder).


License
-------

This project, and its source code, is released under the zlib/libpng license
(see LICENSE.txt). This means that it can be used in any product, commercial
or not.


Version history
---------------

v1.0.6 - 2011.03.29

 - Fixed a potential out-of-bounds bug in the C decoder.
 - Minor changes in the encoder (more consistent choise of marker symbols).


v1.0.5 - 2010.12.01

 - Added a JavaScript version of the decoder.


v1.0.4 - 2010.11.26

 - FIXED A CRITICAL BUG IN THE ENCODER: Offsets equal to 2056 would be encoded
   as 8, resulting in wrong encoded data, and hence also wrong decoded data.
 - Added a Lua version of the decoder.


v1.0.3 - 2010.11.21

 - Improved the encoder speed by a significant amount by doing some acceptable
   trade-offs (earlier termination of the string matching for instance). The
   highest compression level (9) still does a full exhaustive search.
 - Fixed an illegal memory read in the encoder.
 - Added an option to do multiple passes in the benchmark tool.


v1.0.2 - 2010.11.16

 - Added a Pascal version of the decoder.
 - Fixed some errors in the documentation of the data format (encode.c).


v1.0.1 - 2010.11.09

 - Added a simplified reference implementation of the decoder in C.
 - Added assembly language implementations of the decoder for 68k processors
   (16/32-bit architecture) and 6502 processors (8-bit architecture).
 - Added an UNSAFE compile time option for the decoder.
 - Added support for LZO in the benchmark tool (disabled by default, as all the
   other 3rd party compression libraries).


v1.0 - 2010.10.28

 - First release

