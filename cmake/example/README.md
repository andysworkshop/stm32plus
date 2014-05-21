CMake Example
=============

This folder shows a bare-bones example of compiling an external firmware
against stm32plus, using CMake. To compile the example, create a folder
for the out-of-source build tree, configure, and make the firmware binary:

    mkdir build && cd build
    cmake ..
    make blink.bin

The CMakeLists.txt file contains two main things which you might want or
need to change:

  - First, the location of the stm32plus install. If you placed
    it somewhere other than the default, you may need to edit
    CMAKE_PREFIX_PATH.
  - Second, the stm32plus system prefix string, stored in the
    STM32PLUS_CONFIGURATION. The default is for an 8MHz F407
    discovery kit. You will need to change this if you have other
    hardware (and, of course, build/install the appropriate 
    configuration of stm32plus itself). 
