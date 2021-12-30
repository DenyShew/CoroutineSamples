add_executable(server main.cpp)
target_compile_options(server PRIVATE -fcoroutines -ggdb)