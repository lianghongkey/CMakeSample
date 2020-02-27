# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(get_info)
get_cpu_info()

# generate version file by git hash
add_custom_target(
  version_file
  ALL
  COMMAND
     ${CMAKE_COMMAND} -D TARGET_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/git-hash.cmake
  WORKING_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}
 )

# project name and language
project(cmakeexample LANGUAGES CXX VERSION 1.0.0)

message(STATUS "CMAKE_SYSTEM_NAME : ${CMAKE_SYSTEM_NAME}")

# Execute a tiny Python script, Must find PythonInterp first
find_package(PythonInterp REQUIRED)
execute_process(
  COMMAND
    ${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')"
  RESULT_VARIABLE _status
  OUTPUT_VARIABLE _hello_world
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
message(STATUS "python RESULT_VARIABLE is: ${_status}")
message(STATUS "python OUTPUT_VARIABLE is: ${_hello_world}")

# Find PythonLibs first
find_package(PythonLibs REQUIRED)

# we default to Release build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# expose options to the user
option(USE_LIBRARY "Compile sources into a library" OFF)

message(STATUS "Compile sources into a library? ${USE_LIBRARY}")

include(CMakeDependentOption)

# second option depends on the value of the first
cmake_dependent_option(
  MAKE_STATIC_LIBRARY "Compile sources into a static library" OFF
  "USE_LIBRARY" ON
  )

# third option depends on the value of the first
cmake_dependent_option(
  MAKE_SHARED_LIBRARY "Compile sources into a shared library" ON
  "USE_LIBRARY" ON
  )

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# list sources
list(APPEND _sources Message.hpp Message.cpp)
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR} _sources)

foreach(_source_file ${_sources}) 
  message(STATUS "Source file : ${_source_file}") 
endforeach()

if(USE_LIBRARY)
  message(STATUS "Compile sources into a STATIC library? ${MAKE_STATIC_LIBRARY}")
  message(STATUS "Compile sources into a SHARED library? ${MAKE_SHARED_LIBRARY}")

  if(MAKE_SHARED_LIBRARY)
    add_library(message SHARED ${_sources})
    add_executable(hello-world hello-world.cpp)
    target_link_libraries(hello-world message)
  endif()

  if(MAKE_STATIC_LIBRARY)
    add_library(message STATIC ${_sources})
    add_executable(hello-world hello-world.cpp)
    target_link_libraries(hello-world message)
  endif()
else()
  add_executable(hello-world hello-world.cpp ${_sources})
endif()

target_include_directories(hello-world PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(hello-world PRIVATE ${PYTHON_LIBRARIES})
target_compile_definitions(hello-world PUBLIC "IS_LINUX")


add_subdirectory(account)

include(CMakeCPack)

###########################################################################
#########################      T e s t     ################################
###########################################################################


include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git 
  GIT_TAG release-1.8.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  #auto download googletest
  FetchContent_Populate(googletest)
   # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Prevent GoogleTest from using PThreads
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
message(STATUS "googletest_SOURCE_DIR :")
message(STATUS "   ${googletest_SOURCE_DIR}")
message(STATUS "googletest_BINARY_DIR :")
message(STATUS "   ${googletest_BINARY_DIR}")


# example library
add_library(sum_integers SHARED sum_integers.cpp)

add_custom_target(
  gen_doc
  ALL
  COMMAND touch README.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT echo "generator README.md  file."
  )


add_custom_command(
  OUTPUT TestGuid.md 
  COMMAND touch TestGuid.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT echo "generator TestGuid.md  file."
  )
add_custom_target(GenTestGuid DEPENDS TestGuid.md) 


add_executable(cpp_test "")
add_custom_command(
  TARGET cpp_test 
  PRE_BUILD 
  COMMAND echo "This is pre build " 
  ) 
add_custom_command(
  TARGET cpp_test 
  POST_BUILD 
  COMMAND echo "This is post build" 
  )
target_sources(cpp_test PRIVATE test.cpp)
add_dependencies(cpp_test GenTestGuid)
target_link_libraries(cpp_test PRIVATE sum_integers gtest_main)
enable_testing()
add_test(
  NAME google_test
  COMMAND $<TARGET_FILE:cpp_test>
)





