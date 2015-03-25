cmake_minimum_required(VERSION 2.8)

# options
set(MCU_BUILD_TYPE small)
set(MCU_FAMILY f4)
set(MCU_HSE 8000000)
set(STM32_version 040100)


# actual build
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_PREFIX_PATH /usr/local/arm-none-eabi /usr/arm-none-eabi
  CACHE STRING "Library search paths")

set(STM32PLUS_CONFIGURATION ${MCU_BUILD_TYPE}-${MCU_FAMILY}-${MCU_HSE}-hard)
find_package(stm32plus ${STM32_version REQUIRED)
include_directories(${STM32PLUS_INCLUDE_DIRS})

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

add_bin_target(${PROJECT_NAME})
