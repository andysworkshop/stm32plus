# Top level SConstruct file for stm32plus and all the examples.
# This is the one that you need to run.
# 'mode', 'mcu' and 'hse' are required variables:
#
# mode:
#   debug/fast/small. Debug = -O0, Fast = -O3, Small = -Os
#
# mcu:
#   f1hd/f4. f1hd = STM32F103HD series. f4 = STM32F4xx series.
#
# hse:
#   Your external oscillator speed in Hz. Some of the ST standard peripheral library
#   code uses the HSE_VALUE #define that we set here. 
#
# Examples:
#   scons mode=debug mcu=f1hd hse=8000000              // debug / f1hd / 8MHz
#   scons mode=fast mcu=f1hd hse=8000000 install       // fast / f1hd / 8MHz
#   scons mode=small mcu=f4 hse=8000000 install        // small / f4 / 8Mhz
#   scons mode=debug mcu=f4 hse=8000000 -j4 install    // debug / f4 / 8Mhz
#
# The -j<N> option can be passed to scons to do a parallel build. On a multicore
# CPU this can greatly accelerate the build. Set <N> to approximately the number
# of cores that you have.
#
# The built library will end up in the stm32plus/build subdirectory. If you specify
# the install command-line option then that library will be installed into the location
# given by the INSTALLDIR constant (below). Default is /usr/lib/stm32plus/VERSION.
#
# It is safe to compile multiple combinations of mode/mcu/hse as the compiled object
# code and library are placed in a unique directory name underneath stm32plus/build

import os

# set the installation root. you can customise this

VERSION="2.0.0"
INSTALLDIR="/usr/lib/stm32plus/"+VERSION

# get the required args and validate

mode = ARGUMENTS.get('mode')
mcu = ARGUMENTS.get('mcu')
hse = ARGUMENTS.get('hse')

if not (mode in ['debug', 'fast', 'small']):
	print "ERROR: mode must be debug/fast/small"
	Exit(1)

if not (mcu in ['f1hd', 'f4']):
	print "ERROR: mcu must be f1hd/f4"
	Exit(1)

if not hse or not hse.isdigit():
	print "ERROR: hse must be an integer oscillator speed in Hz"
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

env.Append(CPPPATH=["#stm32plus/include","#stm32plus/include/stl","#stm32plus"])

# create the C and C++ flags that are needed. We can't use the extra or pedantic errors on the ST library code.

env.Replace(CCFLAGS=["-Wall","-Werror","-ffunction-sections","-fdata-sections","-fno-exceptions","-mthumb","-gdwarf-2"])
env.Replace(CXXFLAGS=["-Wextra","-Werror","-pedantic-errors","-fno-rtti","-std=gnu++0x","-fno-threadsafe-statics"])
env.Append(CCFLAGS="-DHSE_VALUE="+hse)
env.Append(LINKFLAGS=["-Xlinker","--gc-sections","-mthumb","-g3","-gdwarf-2"])

# add on the MCU-specific definitions

if mcu=="f1hd":
	env.Append(CCFLAGS=["-mcpu=cortex-m3","-DSTM32PLUS_F1_HD"])
	env.Append(ASFLAGS="-mcpu=cortex-m3")
	env.Append(LINKFLAGS="-mcpu=cortex-m3")
elif mcu=="f4":
	env.Append(CCFLAGS=["-mcpu=cortex-m4","-DSTM32PLUS_F4"])
	env.Append(ASFLAGS="-mcpu=cortex-m4")
	env.Append(LINKFLAGS="-mcpu=cortex-m4")

# add on the mode=specific optimisation definitions

if mode=="debug":
	env.Append(CCFLAGS=["-O0","-g3"])
elif mode=="fast":
	env.Append(CCFLAGS=["-O3"])
elif mode=="small":
	env.Append(CCFLAGS=["-Os"])

systemprefix=mode+"-"+mcu+"-"+hse
	
# launch SConscript for the main library

libstm32plus=SConscript("stm32plus/SConscript",
												exports=["mode","mcu","hse","env","systemprefix","INSTALLDIR","VERSION"],
												variant_dir="stm32plus/build/"+systemprefix,
												duplicate=0)

env.Append(LIBS=[libstm32plus])

# Launch SConscript for the examples

SConscript("examples/SConscript",exports=["mode","mcu","hse","env","systemprefix","INSTALLDIR","VERSION"])
