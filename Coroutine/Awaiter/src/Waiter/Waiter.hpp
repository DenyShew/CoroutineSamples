#ifndef __WAITER_H__
#define __WAITER_H__

#include <coroutine>
#include <vector>

template<class S_VAL, class R_VAL, class CORO_TYPE>
class waiter
{
public:
    waiter(std::vector<std::coroutine_handle<CORO_TYPE>>& coro_handler, bool is_ready = false)
        : hanlder(coro_handler)
        , is_ready(is_ready)
    {

    }

    bool await_ready()
    {
        return is_ready;
    }
    S_VAL await_suspend(std::coroutine_handle<CORO_TYPE> coro)noexcept
    {
        handler.emplace_back(coro);
        return suspend_value;
    }
    R_VAL await_resume()noexcept
    {
        return resume_value;
    }

    void set_suspend(const S_VAL& value)
    {
        suspend_value = value;
    }
    void set_resume(const R_VAL& value)
    {
        resume_value = value;
    }
    void set_status(const bool& is_ready)
    {
        this->is_ready = is_ready;
    }
    bool get_status()const
    {
        return is_ready;
    }

protected:

    bool is_ready;
    S_VAL suspend_value;
    R_VAL resume_value;

    std::vector<std::coroutine_handle<CORO_TYPE>>& handler;
};

#endif // __WAITER_H__