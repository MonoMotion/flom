cmake_minimum_required(VERSION 3.0.2)

add_executable(test_effector effector.cpp)
target_link_libraries(test_effector PRIVATE flom_lib rapidcheck)
enable_clang_format(test_effector)
enable_clang_tidy(test_effector)
