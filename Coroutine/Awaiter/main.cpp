#include "task_handler.hpp"

#include <iostream>

task<void> foo()
{
    std::cout << "start" << std::endl;
    co_await task<void>::classic_awaiter(task<void>::classic_awaiter::TYPE_AWAIT::ALWAYS);
    std::cout << "end" << std::endl;
}

int main()
{
    auto val = foo();
    val.resume();
    std::cout << "resuming" << std::endl;
    val.resume();
    return 0;
}