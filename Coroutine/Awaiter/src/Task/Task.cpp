#include "Task.hpp"

void promise_type::set_resume(bool init_resume, bool final_resume)
{
    this->init_resume = init_resume;
    this->final_resume = final_resume;
}

auto promise_type::initial_suspend()
{
    return standart_waiter(true);
}

auto promise_type::final_suspend()
{
    return standart_waiter(false);
}

auto promise_type::get_return_object()
{
    return std::coroutine_handle<promise_type>::from_promise(*this);
}

void promise_type::unhandled_exception()
{
    
}

void promise_type::return_void()
{

}

task::task(std::coroutine_handle<promise_type> coro)
{
    this->coro = coro;
}

task::~task()
{
    this->coro.destroy();
}