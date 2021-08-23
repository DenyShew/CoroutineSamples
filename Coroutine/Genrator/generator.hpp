#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <coroutine>

template<class T>
struct generator
{
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;

    generator(coro_handle handler)
        : handle(handler)
    {   

    }

    bool resume()
    {
        if(!handle.done())handle.resume();
        return !handle.done();
    }

    ~generator()
    {
        handle.destroy();
    }

    bool next()
    {
        return handle ? (handle.resume(), !handle.done()) : false;
    }
    
    T get_value()
    {
        return handle.promise().current_value;
    }

    struct promise_type
    {
        T current_value;
        auto yield_value(T value)
        {
            current_value = value;
            return std::suspend_always{};
        }
        auto get_return_object()
        {
            return coro_handle::from_promise(*this);
        }
        auto initial_suspend()noexcept
        {
            return std::suspend_always{};
        }
        auto final_suspend()noexcept
        {
            return std::suspend_always{};
        }
        void return_void(){}
        void unhandled_exception()
        {

        }
    };

private:
    coro_handle handle;    
};

#endif // __GENERATOR_H__