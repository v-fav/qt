# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\converter_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\converter_autogen.dir\\ParseCache.txt"
  "converter_autogen"
  )
endif()
