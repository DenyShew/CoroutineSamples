cmake_minimum_required(VERSION 3.21)
project(Awaiter CXX)

add_compile_options(-Werror)
add_compile_options(-pedantic-errors)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pg -ggdb -O2 -pg)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fcoroutines")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -fcoroutines")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -fcoroutines")

add_subdirectory(src)
add_subdirectory(app)
