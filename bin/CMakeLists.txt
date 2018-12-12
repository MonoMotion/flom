cmake_minimum_required(VERSION 2.7)

add_executable(load_bin load.cpp)
set_target_properties(load_bin PROPERTIES OUTPUT_NAME "load")
target_link_libraries(load_bin PRIVATE flom_lib nlohmann_json::nlohmann_json)
