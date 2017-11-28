include(${CMAKE_CURRENT_LIST_DIR}/stm32plus-config-${STM32PLUS_CONFIGURATION}.cmake
  OPTIONAL RESULT_VARIABLE STM32PLUS_CONFIG_FILE)

if(NOT "${STM32PLUS_CONFIG_FILE}")
  # If we couldn't find the requested config, try with an "e" appended, for
  # explicit external oscillator. This config name change was made in 4.0.3.
  include(${CMAKE_CURRENT_LIST_DIR}/stm32plus-config-${STM32PLUS_CONFIGURATION}e.cmake)
endif()
