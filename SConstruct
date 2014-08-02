# Top level SConstruct file for stm32plus and all the examples.
"""
Usage: scons mode=<MODE> mcu=<MCU> hse=<HSE> [float=hard]

  <MODE>: debug/fast/small.
    debug = -O0
    fast  = -O3
    small = -Os

  <MCU>: f1hd/f1cle/f1mdvl/f051/f4.
    f1hd   = STM32F103HD series.
    f1cle  = STM32F107 series.
    f1mdvl = STM32100 Medium Density Value Line series.
    f4     = STM32F4xx series.
    f051   = STM32F051 series.

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
    scons mode=small mcu=f4 hse=8000000 -j4 float=hard install  // small / f4 / 8Mhz
    scons mode=debug mcu=f4 hse=8000000 -j4 install             // debug / f4 / 8Mhz
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
"""

import os

# set the installation root. you can customise this. the default attempts to read the
# current release version from the stm32plus.h configuration header file.

VERSION=os.popen('egrep "#define\s+STM32PLUS_BUILD" lib/include/config/stm32plus.h  | sed "s/^.*0x//g"').read()
VERSION=VERSION.rstrip()

if len(VERSION) != 6:
  print "Unexpected error getting the library version"
  Exit(1)

INSTALLDIR=ARGUMENTS.get('INSTALLDIR') or "/usr/local/arm-none-eabi"
INSTALLDIR_PREFIX=ARGUMENTS.get('INSTALLDIR_PREFIX') or "stm32plus-"+VERSION

# get the required args and validate

mode = ARGUMENTS.get('mode')
mcu = ARGUMENTS.get('mcu')
hse = ARGUMENTS.get('hse')
float = None

if not (mode in ['debug', 'fast', 'small']):
	print __doc__
	Exit(1)

if not (mcu in ['f1hd', 'f1cle', 'f4', 'f1mdvl', 'f051']):
	print __doc__
	Exit(1)

if not hse or not hse.isdigit():
	print __doc__
	Exit(1)

print "stm32plus build version is "+VERSION

# set up build environment and pull in OS environment variables

env=Environment(ENV=os.environ)

# replace the compiler values in the environment

env.Replace(CC="arm-none-eabi-gcc")
env.Replace(CXX="arm-none-eabi-g++")
env.Replace(AS="arm-none-eabi-as")
env.Replace(AR="arm-none-eabi-ar")
env.Replace(RANLIB="arm-none-eabi-ranlib")

# set the include directories

env.Append(CPPPATH=["#lib/include","#lib/include/stl","#lib"])

# create the C and C++ flags that are needed. We can't use the extra or pedantic errors on the ST library code.

env.Replace(CCFLAGS=["-Wall","-Werror","-ffunction-sections","-fdata-sections","-fno-exceptions","-mthumb","-gdwarf-2","-pipe"])
env.Replace(CXXFLAGS=["-Wextra","-pedantic-errors","-fno-rtti","-std=gnu++0x","-fno-threadsafe-statics"])
env.Append(CCFLAGS="-DHSE_VALUE="+hse)
env.Append(LINKFLAGS=["-Xlinker","--gc-sections","-mthumb","-g3","-gdwarf-2"])

# add on the MCU-specific definitions

if mcu=="f1hd":
	env.Append(CCFLAGS=["-mcpu=cortex-m3","-DSTM32PLUS_F1_HD"])
	env.Append(ASFLAGS="-mcpu=cortex-m3")
	env.Append(LINKFLAGS="-mcpu=cortex-m3")
elif mcu=="f1cle":
	env.Append(CCFLAGS=["-mcpu=cortex-m3","-DSTM32PLUS_F1_CL_E"])
	env.Append(ASFLAGS="-mcpu=cortex-m3")
	env.Append(LINKFLAGS="-mcpu=cortex-m3")
elif mcu=="f1mdvl":
	env.Append(CCFLAGS=["-mcpu=cortex-m3","-DSTM32PLUS_F1_MD_VL"])
	env.Append(ASFLAGS="-mcpu=cortex-m3")
	env.Append(LINKFLAGS="-mcpu=cortex-m3")
elif mcu=="f4":
	env.Append(CCFLAGS=["-mcpu=cortex-m4","-DSTM32PLUS_F4"])
	env.Append(ASFLAGS="-mcpu=cortex-m4")
	env.Append(LINKFLAGS="-mcpu=cortex-m4")

	# support for the hardware FPU in the F4

	float=ARGUMENTS.get('float')
	if float=="hard":
		env.Append(CCFLAGS=["-mfloat-abi=hard"])
		env.Append(LINKFLAGS=["-mfloat-abi=hard","-mfpu=fpv4-sp-d16"]);
	else:
		float=None

elif mcu=="f051":
	env.Append(CCFLAGS=["-mcpu=cortex-m0","-DSTM32PLUS_F0_51"])
	env.Append(ASFLAGS="-mcpu=cortex-m0")
	env.Append(LINKFLAGS="-mcpu=cortex-m0")

# add on the mode=specific optimisation definitions

if mode=="debug":
	env.Append(CCFLAGS=["-O0","-g3"])
elif mode=="fast":
	env.Append(CCFLAGS=["-O3"])
elif mode=="small":
	env.Append(CCFLAGS=["-Os"])

systemprefix=mode+"-"+mcu+"-"+hse
if float:
	systemprefix += "-"+float
	
# launch SConscript for the main library

libstm32plus=SConscript("lib/SConscript",
												exports=["mode","mcu","hse","env","systemprefix","INSTALLDIR","INSTALLDIR_PREFIX","VERSION"],
												variant_dir="lib/build/"+systemprefix,
												duplicate=0)

env.Append(LIBS=[libstm32plus])

# launch SConscript for the examples

SConscript("examples/SConscript",exports=["mode","mcu","hse","env","systemprefix","INSTALLDIR","INSTALLDIR_PREFIX","VERSION"])

# build the CMake helper

SConscript("cmake/SConscript",
           exports=["env","systemprefix","libstm32plus","INSTALLDIR","INSTALLDIR_PREFIX","VERSION"],
           variant_dir="lib/build/"+systemprefix+"/cmake")
