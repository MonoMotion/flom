cmake_minimum_required(VERSION 2.7)

add_executable(convert_legacy_bin convert_legacy.cpp)
set_target_properties(convert_legacy_bin PROPERTIES OUTPUT_NAME "convert_legacy")
target_link_libraries(convert_legacy_bin PRIVATE flom_lib flom_proto)
enable_clang_format(convert_legacy_bin)
enable_clang_tidy(convert_legacy_bin)

add_executable(json2flom_bin json2flom.cpp)
set_target_properties(json2flom_bin PROPERTIES OUTPUT_NAME "json2flom")
target_link_libraries(json2flom_bin PRIVATE flom_lib flom_proto)
enable_clang_format(json2flom_bin)
enable_clang_tidy(json2flom_bin)

add_executable(flom2json_bin flom2json.cpp)
set_target_properties(flom2json_bin PROPERTIES OUTPUT_NAME "flom2json")
target_link_libraries(flom2json_bin PRIVATE flom_lib flom_proto)
enable_clang_format(flom2json_bin)
enable_clang_tidy(flom2json_bin)
