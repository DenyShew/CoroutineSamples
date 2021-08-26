#include "task_handler.hpp"

task<void> foo()
{
    co_await task<void>::classic_awaiter(task<void>::classic_awaiter::TYPE_AWAIT::ALWAYS);
}

int main()
{
    auto val = foo();
    val.resume();
    return 0;
}