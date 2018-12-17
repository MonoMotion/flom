cmake_minimum_required(VERSION 3.0.2)

function(flom_add_test target)
  target_link_libraries(${target} PRIVATE flom_lib rapidcheck)
  add_test(NAME ${target} COMMAND ${target})
  enable_clang_format(${target})
  enable_clang_tidy(${target})
endfunction()
