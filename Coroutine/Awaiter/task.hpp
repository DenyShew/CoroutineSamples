#ifndef __TASK_H__
#define __TASK_H__

#include <coroutine>
#include <vector>

struct task
{
    struct awaiter;

    struct promise_type;


};

struct task::awaiter
{
    enum class TYPE_AWAIT
    {
        ALWAYS, NEVER, CLASSIC
    };

    awaiter(TYPE_AWAIT type)
    {
        switch(type)
        {
            case TYPE_AWAIT::ALWAYS:
            {
                is_resume = false; 
            }
            case TYPE_AWAIT::NEVER:
            {
                is_resume = true;
            }
            case TYPE_AWAIT::CLASSIC:
            {
                is_resume = false;
            }
        }
    }

    bool await_ready()
    {
        return is_resume;
    }
    auto await_suspend(std::coroutine_handle<task::promise_type> coro)noexcept
    {

    }
    auto await_resume()noexcept
    {

    }

private:
    bool is_resume;
};

struct task::promise_type
{
    auto get_return_object()
    {
        return std::coroutine_handle<promise_type>::from_promise(*this);
    }
    auto ititial_suspend()
    {

    }
    auto final_suspend()
    {

    }
};

#endif // __TASK_H__