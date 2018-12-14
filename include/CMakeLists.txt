file(GLOB HEADER_FILES flom/*.hpp)
add_custom_target(flom_headers SOURCES ${HEADER_FILES})
enable_clang_format(flom_headers)
enable_clang_tidy(flom_headers)
