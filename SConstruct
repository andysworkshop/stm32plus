# Top level SConstruct file for stm32plus and all the examples.
"""
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
    f405   = STM32F405
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
"""

import os


#
# set CCFLAGS/ASFLAGS/LINKFLAGS
#

def setFlags(cpu,libdef):

  cpuopt="-mcpu=cortex-"+cpu;
  libopt="-DSTM32PLUS_"+libdef;

  env.Append(CCFLAGS=[cpuopt,libopt])
  env.Append(ASFLAGS=cpuopt)
  env.Append(LINKFLAGS=cpuopt)

#
# set the F4-specific hard float option
#

def floatOpt():
  global float
  float=ARGUMENTS.get('float')
  if float=="hard":
    env.Append(CCFLAGS=["-mfloat-abi=hard"])
    env.Append(LINKFLAGS=["-mfloat-abi=hard","-mfpu=fpv4-sp-d16"]);
  else:
    float=None

  return


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

if not hse or not hse.isdigit():
  print __doc__
  Exit(1)

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
  setFlags("m3","F1_HD")
elif mcu=="f1cle":
  setFlags("m3","F1_CL_E")
elif mcu=="f1mdvl":
  setFlags("m3","F1_MD_VL")
elif mcu=="f4" or mcu=="f407":
  setFlags("m4","F407")
  floatOpt()
elif mcu=="f405":
  setFlags("m4","F405")
  floatOpt()
elif mcu=="f415":
  setFlags("m4","F415")
  floatOpt()
elif mcu=="f417":
  setFlags("m4","F417")
  floatOpt()
elif mcu=="f427":
  setFlags("m4","F427")
  floatOpt()
  floatOpt()
elif mcu=="f437":
  setFlags("m4","F437")
  floatOpt()
elif mcu=="f429":
  setFlags("m4","F429")
  floatOpt()
elif mcu=="f439":
  setFlags("m4","F439")
  floatOpt()
elif mcu=="f051":
  setFlags("m0","F0_51")
elif mcu=="f030":
  setFlags("m0","F0_30")
else:
  print __doc__
  Exit(1)

# add on the mode=specific optimisation definitions

if mode=="debug":
  env.Append(CCFLAGS=["-O0","-g3"])
elif mode=="fast":
  env.Append(CCFLAGS=["-O3"])
elif mode=="small":
  env.Append(CCFLAGS=["-Os"])
else:
  print __doc__
  Exit(1)

print "stm32plus build version is "+VERSION

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
