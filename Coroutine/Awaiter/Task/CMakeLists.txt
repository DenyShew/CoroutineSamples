add_library(Task STATIC Task.hpp Task.cpp)

target_link_libraries(Task Waiter)