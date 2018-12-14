cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0048 NEW)

project(flom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-pedantic -Wall -Weverything -Wno-float-equal -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-missing-prototypes -Wno-range-loop-analysis)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-pedantic -Wall -Wextra -Wno-float-equal -Wno-padded -Wno-unused-parameter -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef)
endif()

if(NOT CONFIG)
  set(CONFIG Release)
endif()

if(CONFIG STREQUAL "Debug")
  add_compile_options(-O0 -g3)
elseif(CONFIG STREQUAL "Release")
  add_compile_options(-O3)
endif()

set(PROTO_DIR ./proto)
set(PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})

include(FindProtobuf)
set(Protobuf_IMPORT_DIRS ${PROTO_DIR})
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
file(GLOB PROTO_FILES ${PROTO_DIR}/*.proto)
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
add_library(flom_proto ${PROTO_HEADER} ${PROTO_SRC})
set_target_properties(flom_proto PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(flom_proto ${PROTOBUF_LIBRARIES})
include_directories(${PROTO_INCLUDE_DIR})

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/nlohmann_json)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)

add_subdirectory(include/)
include_directories(include/)

add_subdirectory(lib)
add_subdirectory(bin)
