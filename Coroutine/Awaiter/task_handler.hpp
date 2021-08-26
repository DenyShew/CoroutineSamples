#include "task.hpp"

#include <vector>

template<typename T>
struct task_handler
{

private:
    std::vector<task<T>> tasks;    
};
