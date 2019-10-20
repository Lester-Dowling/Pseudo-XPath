cmake_minimum_required(VERSION 3.15)
project(
  "Pseudo-XPath"
  VERSION 0.5.0
  DESCRIPTION
    "A simplied, custom XPath-based grammar which is sufficient to filter Excel table data."
)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} OBJECT)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Public files and source code files
#
file(
  GLOB_RECURSE SRC_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file(
  GLOB_RECURSE PUB_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES} PUBLIC ${PUB_FILES})
foreach(src_file ${SRC_FILES})
  message(STATUS "${PROJECT_NAME} src file: ${src_file}")
endforeach(src_file)
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

# Private include directories:
target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
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
# Import String-Extra git submodule
#
if(NOT TARGET "String-Extra")
  add_subdirectory("String-Extra")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "String-Extra")

#
# Boost
#
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC #
         Boost::boost #
         Boost::filesystem #
         Boost::date_time #
         Boost::regex #
)

#
# Unit tests Only build unit tests if this project isn't a submodule in a super-
# project.
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  message(STATUS "Building unit tests: ${BUILD_TESTING}")
  if(BUILD_TESTING)
    add_subdirectory("Unit-Tests")
  endif()
endif()
