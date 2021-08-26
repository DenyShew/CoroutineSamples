#ifndef __TASK_H__
#define __TASK_H__

#include <coroutine>

template<typename T>
struct task;

//Asymmetric coroutine
template<>
struct task<void>
{
    struct promise_type;

    struct awaiter
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
        void await_suspend(std::coroutine_handle<promise_type> coro)noexcept
        {
            
        }
        auto await_resume()noexcept
        {

        }

    protected:
        bool is_resume;
    };

    struct promise_type
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
        auto return_value()
        {

        }
        void return_void()
        {

        }
        void unhandled_exception()
        {
            
        }
    };

    task(std::coroutine_handle<promise_type> coro)
        : handle(coro)
    {

    }

    ~task()
    {
        handle.destroy();
    }

    std::coroutine_handle<promise_type> get_handle()
    {
        return this->handle;
    }

protected:
    std::coroutine_handle<promise_type> handle;
};

//Symmetric coroutine
template<typename T>
struct task
{
    struct promise_type;

    struct awaiter
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
        auto await_suspend(std::coroutine_handle<promise_type> coro)noexcept
        {
            return coro;
        }
        auto await_resume()noexcept
        {

        }

    protected:
        bool is_resume;
    };

    struct promise_type
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
        auto return_value()
        {

        }
        void return_void()
        {

        }
        void unhandled_exception()
        {
            
        }
    };

    task(std::coroutine_handle<promise_type> coro)
        : handle(coro)
    {

    }

    ~task()
    {
        handle.destroy();
    }

    std::coroutine_handle<promise_type>& get_handle()
    {
        return handle;
    }

protected:
    std::coroutine_handle<promise_type> handle;
};

#endif // __TASK_H__