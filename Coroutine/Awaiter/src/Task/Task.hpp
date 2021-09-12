#ifndef __TASK_H__
#define __TASK_H__

#include "../Waiter/Waiter.hpp"

class promise_type
{
public:
    void set_resume(bool init_resume = false, bool final_resume = true);

    auto initial_suspend();
    auto final_suspend();
    auto get_return_object();
    void unhandled_exception();
    void return_void();
protected:
    bool init_resume;
    bool final_resume;

    coro_set<std::coroutine_handle<promise_type>>* coro;
};

class task
{
public:
    task(std::coroutine_handle<promise_type> coro);
    ~task();

protected:
    std::coroutine_handle<promise_type> coro;
};

#endif // __TASK_H__