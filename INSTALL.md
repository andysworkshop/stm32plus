How to build and install stm32plus
==================================

This readme will help you to understand how you can build and install stm32plus.

Get the source
--------------

Firstly you need to clone the repo:

	git clone https://github.com/andysworkshop/stm32plus.git

If you're happy with building from the `master` branch then you can just go right ahead to the next step. Otherwise, you'll need to use git to checkout the tag or branch you want to build from. Tags represent releases and are the safest build option. The `master` branch is the next safest and the feature branches are for the brave.

Prerequisites
-------------

* A compatible arm-none-eabi toolchain. I prefer the [ARM launchpad](https://launchpad.net/gcc-arm-embedded) toolchain because it supports the hardware FPU in the F4 series. I can also confirm that the Mentor Graphics (formerly CodeSourcery) _Sourcery G++ Lite_ toolchain preferred in previous stm32plus releases still works as long as you don't require the hardware FPU. Download the most recent EABI version, install it on your system and ensure that it's in your path by attempting to execute one of the commands:

		$ arm-none-eabi-g++
		arm-none-eabi-g++.exe: no input files

* `scons`. Writing Makefiles to cope with all the variant builds was driving me crazy so I switched to _scons_ and have never looked back. If you don't already have it then google it and download and install it on to your system from the official website. Linux users can get it using their official package management interface (e.g. `apt-get`)

### Additional prerequisites for Windows systems ###
   

* cygwin, msys or any other system that makes your build environment look like Unix. I use cygwin. Attempting to build out of something as lobotomised as a Windows _cmd_ shell is not supported.

* Windows builds under cygwin _must_ use the cygwin supplied python and the source build of scons. Get python from the official cygwin update site. Get scons from [http://www.scons.org/download.php](http://www.scons.org/download.php) and select _Zip file_ or _Gzip tar file_ and install it using `setup.py` from the cygwin bash prompt.

_Do not_ get windows installer versions of python or scons. They are not compatible with cygwin.

How to build
------------

* Change directory to the top-level directory containing the `SConstruct` file.

* stm32plus supports the F0, F100 MDVL, F103 HD, F107 CL and F4 series MCUs. Decide which one you want to build for. You also need to know your external oscillator (HSE) speed and you need to decide on whether you want to build a debug (-O0), small (-Os) or fast (-O3) library. If you're using a device that does not have an external oscillator (e.g. the F0 discovery board) then just use 8000000 as a default.

You can build all of the above combinations side-by-side if you so wish by executing `scons` multiple times.

* Execute `scons` with the parameters that define the build:

		Usage: scons mode=<MODE> mcu=<MCU> hse=<HSE> [float=hard]

		  <MODE>: debug/fast/small.
    		debug = -O0
    		fast  = -O3
    		small = -Os

  		<MCU>: f1hd/f1cle/f1mdvl/f051/f030/f4.
    		f030   = STM32F030 series.
    		f051   = STM32F051 series.
    		f1hd   = STM32F103HD series.
    		f1cle  = STM32F107 series.
    		f1mdvl = STM32100 Medium Density Value Line series.
    		f4     = STM32F407/f417 series (maintained for backwards compatibility)
    		f401   = STM32F401
    		f407   = STM32F407
    		f415   = STM32F417
    		f417   = STM32F417
    		f427   = STM32F427
    		f437   = STM32F437
    		f429   = STM32F429
    		f439   = STM32F439

  		<HSE>:
    		Your external oscillator speed in Hz. Some of the ST standard peripheral
    		library code uses the HSE_VALUE #define that we set here. If you're using
    		the HSI and don't have an HSE connected then just supply a default
    		of 8000000.

  		[float=hard]:
    		Optional flag for an F4 build that will cause the hardware FPU to be
    		used for floating point operations. Requires the "GNU Tools for ARM Embedded
    		Processors" toolchain. Will not work with Code Sourcery Lite.

  		Examples:
    		scons mode=debug mcu=f1hd hse=8000000                       // debug / f1hd / 8MHz
    		scons mode=debug mcu=f1cle hse=25000000                     // debug / f1cle / 25MHz
    		scons mode=debug mcu=f1mdvl hse=8000000                     // debug / f1mdvl / 8MHz
    		scons mode=fast mcu=f1hd hse=8000000 install                // fast / f1hd / 8MHz
    		scons mode=small mcu=f4 hse=8000000 -j4 float=hard install  // small / f407 or f417 / 8Mhz
    		scons mode=debug mcu=f4 hse=8000000 -j4 install             // debug / f407 or f417 / 8Mhz
    		scons mode=debug mcu=f051 hse=8000000 -j4 install           // debug / f051 / 8Mhz

  		Additional Notes:
    		The -j<N> option can be passed to scons to do a parallel build. On a multicore
    		CPU this can greatly accelerate the build. Set <N> to approximately the number
    		of cores that you have.

    		The built library will be placed in the stm32plus/build subdirectory.

    		If you specify the install command-line option then that library will be installed
    		into the location given by INSTALLDIR, which defaults to /usr/local/arm-none-eabi.
    		The library, headers, and examples will be installed respectively, to the lib,
    		include, and bin subdirectories of INSTALLDIR.

    		It is safe to compile multiple combinations of mode/mcu/hse as the compiled object
    		code and library are placed in a unique directory name underneath stm32plus/build.
    		It is likewise safe to install multiple versions of the library and examples.

The `-j<N>` option can be passed to scons to do a parallel build. On a multicore CPU this can greatly accelerate the build. Set <N> to approximately the number of cores that you have.

The `install` option will install the library and the examples into subdirectories of `/usr/local/arm-none-eabi`. This location can be customised by supplying an `INSTALLDIR` argument on the command line.

#### A note on the example projects ####

The example projects are designed to run on either the 512/64Kb/72MHz STM32F103, the 256/64Kb/72Mhz STM32F107, the 128/8Kb/24MHz STM32F100, the 1024Kb/128Kb/168Mhz STM32F4 and the 64/8Kb/48MHz F051.

It is the linker script (`Linker.ld`) and the system startup code (`System.c`) that specify these things. For example, if you wanted to change the core clock then you need to look at `System.c` (`SystemCoreClock` is a key variable). If you want to change the memory size then you need to look at `Linker.ld`. The stm32plus library itself is clock-speed and memory-independent. For example, I have used stm32plus with an STM32F429 MCU just by using the F4 build and adjusting my system and linker files to reflect the higher clock speed and memory configuration.

Some examples are not suitable for all MCUs. For example, the STM32F107 does not come with SDIO or FSMC peripherals, and the STM32F103 does not have an ethernet MAC. If an example is not suitable for the MCU that you are targetting then the scons script will skip over it and the Eclipse project will not contain a configuration for it.

#### A note on the net examples

The network code requires the dynamic heap CRT library functions to be safe for re-entrant use, therefore these examples contain the following code in the `LibraryHacks.cpp` file. When writing your own code it is very important that you include this:

	/*
	 * The net code needs the heap to re-entrant so we need to ensure that an
	 * IRQ cannot be raised while the heap structures are updated
	 */
	
	extern "C" {
	  void __malloc_lock(struct _reent * /* reent */) {
	    stm32plus::IrqSuspend::suspend();
	  }
	
	  void __malloc_unlock(struct _reent * /* reent */) {
	    stm32plus::IrqSuspend::resume();
	  }
	}
