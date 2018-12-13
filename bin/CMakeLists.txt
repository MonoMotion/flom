cmake_minimum_required(VERSION 2.7)

add_executable(convert_legacy_bin convert_legacy.cpp)
set_target_properties(convert_legacy_bin PROPERTIES OUTPUT_NAME "convert_legacy")
target_link_libraries(convert_legacy_bin PRIVATE flom_lib flom_proto)
