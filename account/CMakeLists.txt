include(FetchContent)

FetchContent_Declare(
  pybind11_sources
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.2
  )
FetchContent_GetProperties(pybind11_sources)
if(NOT pybind11_sources_POPULATED)
  FetchContent_Populate(pybind11_sources)
  add_subdirectory(
    ${pybind11_sources_SOURCE_DIR}
    ${pybind11_sources_BINARY_DIR}
    )
endif()
message(STATUS "pybind11_sources_SOURCE_DIR :")
message(STATUS "   ${pybind11_sources_SOURCE_DIR}")
message(STATUS "pybind11_sources_BINARY_DIR :")
message(STATUS "   ${pybind11_sources_BINARY_DIR}")



# example library
add_library(account MODULE account.cpp)
target_link_libraries(account PUBLIC  pybind11::module)
set_target_properties(account
  PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
  )


# turn on testing
enable_testing()

# define test
add_test(
  NAME
    python_test
  COMMAND
    ${CMAKE_COMMAND} -E env ACCOUNT_MODULE_PATH=$<TARGET_FILE_DIR:account>
    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test.py
  )