#
# Copyright 2018 coord.e
#
# This file is part of Flom.
#
# Flom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Flom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Flom.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.7.2)
cmake_policy(SET CMP0048 NEW)

project(flom LANGUAGES CXX)

execute_process(COMMAND ${PROJECT_SOURCE_DIR}/version.sh simple OUTPUT_VARIABLE flom_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
project(flom VERSION ${flom_VERSION})

execute_process(COMMAND ${PROJECT_SOURCE_DIR}/version.sh OUTPUT_VARIABLE flom_DETAILED_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Version: v${flom_DETAILED_VERSION}")

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.8.2")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++17 HAS_STD_CXX17)
  if(${HAS_STD_CXX17})
    set(CMAKE_CXX_FLAGS -std=c++17)
  else()
    set(CMAKE_CXX_FLAGS -std=c++1z)
  endif()
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CONFIG)
  set(CONFIG Release)
endif()

set(INCLUDE_INSTALL_DIR include CACHE PATH "installation directory of header files, relative to ${CMAKE_INSTALL_PREFIX}")
set(LIB_INSTALL_DIR lib CACHE PATH "installation directory of library files, relative to ${CMAKE_INSTALL_PREFIX}")
option(ENABLE_TEST "Build and run test cases" ON)
option(USE_STATIC_PROTOBUF "Link against static version of protobuf library" OFF)
option(USE_PIC "Use -fPIC (Position independent code)" ON)

message(STATUS "Build in ${CONFIG} mode")

if(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

set(PROTO_DIR ./proto)
set(PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(Protobuf_USE_STATIC_LIBS ${USE_STATIC_PROTOBUF})

include(FindProtobuf)
set(Protobuf_IMPORT_DIRS ${PROTO_DIR})
find_package(Protobuf 3.0.0 REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
file(GLOB PROTO_FILES ${PROTO_DIR}/*.proto)
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
add_library(flom_proto ${PROTO_HEADER} ${PROTO_SRC})
set_target_properties(flom_proto PROPERTIES POSITION_INDEPENDENT_CODE ${USE_PIC})
target_link_libraries(flom_proto INTERFACE ${PROTOBUF_LIBRARIES})
include_directories(SYSTEM ${PROTO_INCLUDE_DIR})

find_package(Boost 1.62 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_subdirectory(third_party/rapidcheck EXCLUDE_FROM_ALL)
include_directories(SYSTEM third_party/rapidcheck/include)
include_directories(SYSTEM third_party/rapidcheck/extras/boost_test/include)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)
include(cmake/compile-options.cmake)
include(cmake/bin.cmake)
include(cmake/lib.cmake)
include(cmake/cpack.cmake)

add_subdirectory(include/)
include_directories(include/)

add_subdirectory(lib)
add_subdirectory(bin)

add_subdirectory(config/)

if(${ENABLE_TEST})
  enable_testing()
  add_subdirectory(test)
endif()
