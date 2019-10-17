cmake_minimum_required (VERSION 3.15)
project (
  "Pseudo-XPath"
  VERSION 0.5.0
  DESCRIPTION "A simplied, custom XPath-based grammar which is sufficient to filter Excel table data."
  )
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library( ${PROJECT_NAME} OBJECT )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Public files and source code files
#
file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file(GLOB_RECURSE PUB_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME}
  PRIVATE ${SRC_FILES}
  PUBLIC  ${PUB_FILES}
  )
foreach(src_file ${SRC_FILES})
  message(STATUS "${PROJECT_NAME} src file: ${src_file}")
endforeach(src_file)
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

# Private include directories:
target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
  target_compile_definitions(
	${PROJECT_NAME}
    PUBLIC
	UNICODE
	_UNICODE
	_CONSOLE
	NOMINMAX
	WIN64
	_WIN64
	WINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP
	_WIN32_WINNT=_WIN32_WINNT_WIN10
	_CRT_SECURE_NO_WARNINGS
	_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  target_compile_options(${PROJECT_NAME} PRIVATE -GR) # Turn on RTTI.
endif()

#
# Parent directory for locally cached repositories of open source software (oss)
#
option(GRANT_INTERNET_ACCESS "Permit CMake to download from the Internet" OFF)
message(STATUS "Access Internet permission: ${GRANT_INTERNET_ACCESS}")
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  get_filename_component(HOME $ENV{USERPROFILE} ABSOLUTE)
else()
  get_filename_component(HOME $ENV{HOME} ABSOLUTE)
endif()
get_filename_component(LOCALLY_CACHED_REPOS_DIR "${HOME}/oss" ABSOLUTE)
if(EXISTS ${LOCALLY_CACHED_REPOS_DIR})
  message(STATUS "Locally cached repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
else()
  message(WARNING "No local repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
endif()

#
# Pre-compiled Header
#
# Downloaded external project https://github.com/larsch/cmake-precompiled-header
#
get_filename_component(
  LOCAL_PRECOMPILED_CMAKE
  "${LOCALLY_CACHED_REPOS_DIR}/cmake-precompiled-header/PrecompiledHeader.cmake" ABSOLUTE)
if(EXISTS ${LOCAL_PRECOMPILED_CMAKE})
  message(STATUS "PrecompiledHeader.cmake: " ${LOCAL_PRECOMPILED_CMAKE})
  include(${LOCAL_PRECOMPILED_CMAKE})
  target_sources(${PROJECT_NAME} PRIVATE "pch-xpath-parser.cpp")
  add_precompiled_header(${PROJECT_NAME} "pch-xpath-parser.hpp" SOURCE_CXX "pch-xpath-parser.cpp")
endif()

#
# Import String-Extra git submodule
#
add_subdirectory("String-Extra")
target_link_libraries(${PROJECT_NAME} PRIVATE "String-Extra")

#
# Boost
#
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  Boost::boost
  Boost::filesystem
  Boost::date_time
  Boost::regex
  )

#
# Unit tests
# Only build unit tests if this project isn't a submodule in a super-project.
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  message(STATUS "Building unit tests: ${BUILD_TESTING}")
  if(BUILD_TESTING)
    add_subdirectory("Unit-Tests")
  endif()
endif()