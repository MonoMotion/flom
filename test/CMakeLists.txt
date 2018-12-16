cmake_minimum_required(VERSION 3.0.2)

add_executable(test_effector effector.cpp)
flom_add_test(test_effector)

add_executable(test_frame frame.cpp)
flom_add_test(test_frame)

add_executable(test_motion motion.cpp)
flom_add_test(test_motion)
