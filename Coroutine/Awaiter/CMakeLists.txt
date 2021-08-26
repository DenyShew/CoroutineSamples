SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fcoroutines")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -fcoroutines")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -fcoroutines")

add_executable(Generator main.cpp task.hpp)