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

    struct classic_awaiter
    {
        enum class TYPE_AWAIT
        {
            ALWAYS, NEVER, CLASSIC
        };

        classic_awaiter(TYPE_AWAIT type)
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
    };

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
            return classic_awaiter{};
        }
        auto final_suspend()
        {
            return classic_awaiter{};
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
        
    protected:
        awaiter wt;
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

    struct classic_awaiter
    {
        enum class TYPE_AWAIT
        {
            ALWAYS, NEVER, CLASSIC
        };

        classic_awaiter(TYPE_AWAIT type)
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
    };

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
            //todo some code...
            return handle;
        }
        auto await_resume()noexcept
        {

        }

        void set_handle(std::coroutine_handle<promise_type> coro)
        {
            this->handle = coro;
        }

    protected:
        bool is_resume;
        std::coroutine_handle<promise_type> handle;
    };

    struct promise_type
    {
        auto get_return_object()
        {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        auto ititial_suspend()
        {
            return classic_awaiter{};
        }
        auto final_suspend()
        {
            return classic_awaiter{};
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

    protected:

        awaiter wt;
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