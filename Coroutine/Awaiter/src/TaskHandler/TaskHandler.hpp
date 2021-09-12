#ifndef __TASKHANDER_H__
#define __TASKHANDER_H__

#include <algorithm>
#include <coroutine>
#include <thread>
#include <mutex>
#include <vector>

template<class CORO_TYPE>
class coro_set
{
public:
    bool is_ready(const size_t& index)const
    {
        return *(ready[index]);
    }
    size_t size()const
    {
        return coro.size();
    }
    void resume(const size_t& index)
    {
        m.lock();
        coro[index].resume();
        m.unlock();
    }
    void resume_all_if()
    {
        m.lock();
        for(int i=0;i<coro.size();i++)
        {
            if(*(ready[i]))
            {
                coro[i].resume();
            }
        }
        m.unlock();
    }
    void add(std::coroutine_handle<CORO_TYPE> coro, bool* is_ready)
    {
        m.lock();
        coro.emplace_back(coro);
        ready.emplace_back(is_ready);
        m.unlock();
    }
protected:
    std::mutex m;
    std::vector<std::coroutine_handle<CORO_TYPE>> coro;
    std::vector<bool*> ready;
};

#endif // __TASKHANDER_H__