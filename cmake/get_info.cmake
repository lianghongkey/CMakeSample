# processor information

macro(get_cpu_info)
  cmake_host_system_information(RESULT _processor_description QUERY PROCESSOR_DESCRIPTION)
  message(STATUS "PROCESSOR_DESCRIPTION : ${_processor_description}")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cpu_info.list.in ${CMAKE_CURRENT_SOURCE_DIR}/generated/cpu_info.list @ONLY)
endmacro()