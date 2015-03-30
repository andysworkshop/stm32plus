cmake_minimum_required(VERSION 2.8)

# Find stm32plus installed library.
set(CMAKE_PREFIX_PATH /usr/local/arm-none-eabi /usr/arm-none-eabi
  CACHE STRING "Library search paths")
set(MCU_FAMILY f4)
set(STM32PLUS_CONFIGURATION small-${MCU_FAMILY}-8000000)
find_package(stm32plus REQUIRED)
include_directories(${STM32PLUS_INCLUDE_DIRS})

# Suppress -rdynamic to the compiler. This should not be necessary
# if your toolchain is gcc-arm-embedded.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# Declare the project name and executable target.
project(blink C CXX ASM)
add_executable(${PROJECT_NAME}
  blink.cpp
  system/LibraryHacks.cpp
  system/${MCU_FAMILY}/System.c
  system/${MCU_FAMILY}/Startup.s
)
target_link_libraries(${PROJECT_NAME} ${STM32PLUS_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES
  LINK_FLAGS "-T${PROJECT_SOURCE_DIR}/system/${MCU_FAMILY}/Linker.ld")

# Generate a bin file from GCC's elf output.
add_bin_target(${PROJECT_NAME})
