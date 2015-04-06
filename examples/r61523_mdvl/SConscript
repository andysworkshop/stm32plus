#
# SConscript build file for an stm32plus example. Called automatically by the SConstruct
# in the parent directory
#

import os

# import everything exported in SConstruct

Import('*')

# get a copy of the environment

env=env.Clone()

# verify that this example is compatible with the selected MCU

sconscriptFile=(lambda x:x).func_code.co_filename
sconscriptDir=os.path.dirname(sconscriptFile)
compatFile=os.path.join(sconscriptDir,"compat.txt");

supported=True
if os.path.exists(compatFile):
  supported=False
  for line in open(compatFile,"r"):
    line=line.rstrip()
    if not line.startswith("#") and line==mcu:
      supported=True
      break

if supported:

  # get all the sources

  matches=[]
  matches.append(Glob("*.cpp"))
  matches.append(Glob("*.c"))
  matches.append(Glob("*.asm"))
  matches.append("system/LibraryHacks.cpp")

  # handle MCU differences

  if mcu.startswith("f4"):
    matches.append("system/f407_168_8/Startup.asm")
    matches.append("system/f407_168_8/System.c")
    linkerscript="examples/"+example+"/system/f407_168_8/Linker.ld"

  elif mcu=="f1hd":
    matches.append("system/f1hd_72_8/Startup.asm")
    matches.append("system/f1hd_72_8/System.c")
    linkerscript="examples/"+example+"/system/f1hd_72_8/Linker.ld"

  elif mcu=="f1cle":
    matches.append("system/f107_72_8/Startup.asm")
    matches.append("system/f107_72_8/System.c")
    linkerscript="examples/"+example+"/system/f107_72_8/Linker.ld"

  elif mcu=="f1mdvl":
    matches.append("system/f1mdvl_24_8/Startup.asm")
    matches.append("system/f1mdvl_24_8/System.c")
    linkerscript="examples/"+example+"/system/f1mdvl_24_8/Linker.ld"

  elif mcu=="f051":
    matches.append("system/f051_48_8/Startup.asm")
    matches.append("system/f051_48_8/System.c")
    linkerscript="examples/"+example+"/system/f051_48_8/Linker.ld"

  elif mcu=="f030":
    matches.append("system/f030_48_8/Startup.asm")
    matches.append("system/f030_48_8/System.c")
    linkerscript="examples/"+example+"/system/f030_48_8/Linker.ld"

  buildoutdir="examples/"+example+"/build/"+systemprefix

  # set the additional linker flags

  env.Append(LINKFLAGS=["-T"+linkerscript,"-Wl,-wrap,__aeabi_unwind_cpp_pr0","-Wl,-wrap,__aeabi_unwind_cpp_pr1","-Wl,-wrap,__aeabi_unwind_cpp_pr2"])

  # additional include directory for the graphics

  env.Append(ASFLAGS="-Iexamples/"+example)

  # unique additions for this example

  env.Append(CPPPATH="#examples/"+example)

  # trigger a build with the correct library name

  elf=env.Program(example+".elf",matches)
  hex=env.Command(example+".hex",elf,"arm-none-eabi-objcopy -O ihex "+buildoutdir+"/"+example+".elf "+buildoutdir+"/"+example+".hex")
  bin=env.Command(example+".bin",elf,"arm-none-eabi-objcopy -O binary "+buildoutdir+"/"+example+".elf "+buildoutdir+"/"+example+".bin")
  lst=env.Command(example+".lst",elf,"arm-none-eabi-objdump -h -S "+buildoutdir+"/"+example+".elf > "+buildoutdir+"/"+example+".lst")
  size=env.Command(example+".size",elf,"arm-none-eabi-size --format=berkeley "+buildoutdir+"/"+example+".elf | tee "+buildoutdir+"/"+example+".size")

  # install the library if the user gave the install option

  EXAMPLEINSTALLDIR=INSTALLDIR+"/examples/"+example+"/"+systemprefix
  env.Alias("install",env.Install(EXAMPLEINSTALLDIR,[elf,hex,bin,lst,size]))
