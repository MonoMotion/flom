cmake_minimum_required(VERSION 2.7)

add_library(flom_lib SHARED motion.cpp frame.cpp effector.cpp proto_util.cpp)
set_target_properties(flom_lib PROPERTIES OUTPUT_NAME "flom")
set_target_properties(flom_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(flom_lib PRIVATE flom_proto nlohmann_json::nlohmann_json)
