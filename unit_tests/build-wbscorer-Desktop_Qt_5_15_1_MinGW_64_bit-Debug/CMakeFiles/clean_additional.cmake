# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\scorer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\scorer_autogen.dir\\ParseCache.txt"
  "scorer_autogen"
  )
endif()
