#include <coroutine>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <functional>

class task;

class io_handler;

template<typename T>
class awaiter;

class tcp_socket;

class accept_socket;

//sock-index
struct socket_info
{
    int sock;
    int index;
};

socket_info unpack(uint64_t val)
{
    socket_info info;
    info.index = int(val);
    info.sock = val >> 32;
    return info;
}

uint64_t pack(socket_info val)
{
    uint64_t rtr = 0;
    rtr |= val.sock;
    rtr = rtr << 32;
    rtr |= val.index;
    return rtr;
}

class task
{
public:
    struct promise_type
    {
        auto get_return_object()
        {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        auto initial_suspend()noexcept
        {
            return std::suspend_never{};
        }
        auto final_suspend()noexcept
        {
            return std::suspend_always{};
        }
        void return_void()
        {

        }
        void unhandled_exception()
        {
            
        }
    };
public:
    task(std::coroutine_handle<promise_type> coro)
    {

    }
};

struct io_task
{
    struct elementary_io_task
    {
        int index;
        std::coroutine_handle<task::promise_type> coro;
    };
    elementary_io_task read;
    elementary_io_task write;
};

class io_handler
{
friend class tcp_socket;
friend class accept_socket;
public:
    io_handler(int size)
    {
        epoll_fd = epoll_create1(0);
        events_size = size;
        events = new struct epoll_event[size];
    }
    void run()
    {
        int size = epoll_wait(epoll_fd, events, events_size, -1);
        for(int i=0;i<size;i++)
        {
            socket_info info = unpack(events[i].data.u64);
            if((events[i].events & EPOLLERR) == EPOLLERR)
            {
                //TODO
                continue;
            }
            if((events[i].events & EPOLLHUP) == EPOLLHUP)
            {
                //TODO
                continue;
            }
            tasks[info.index].resume();
        }
    }
    void set_coro_by_index(int index, std::coroutine_handle<task::promise_type> coro)
    {
        tasks[index] = coro;
    }
private:
    //return index
    io_task add_sock()
    {
        io_task rtr;
        rtr.read.index = tasks.size();
        rtr.write.index = rtr.read.index + 1;
        tasks.resize(tasks.size() + 2);
        return rtr;
    }
private:
    int events_size;
    int epoll_fd;
    struct epoll_event* events;
    std::vector<std::coroutine_handle<task::promise_type>> tasks;
};

template<typename T>
class awaiter
{
public:
    awaiter(io_handler& handler, int index, const std::function<T()>& on_resume)
        : handler(handler)
        , index(index)
        , on_resume(on_resume)
    {

    }
    void await_suspend(std::coroutine_handle<task::promise_type> coro)noexcept
    {
        handler.set_coro_by_index(index, coro);
    }
    bool await_ready()noexcept
    {
        return false;
    }
    T await_resume()noexcept
    {
        return on_resume();
    }
private:
    io_handler& handler;
    int index;
    std::function<T()> on_resume;
};

class tcp_socket
{
public:
    
};

class accept_socket
{
public:

};

void setnonblocking(int sock)
{
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFD, 0) | O_NONBLOCK);
}

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        return 1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(34250);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        return 2;
    }
    if(listen(listener, 1)==-1)
    {
        return 3;
    }
    sock = accept(listener, 0, 0);
    if(sock == -1)
    {
        return 4;
    }
    std::cout << "Accepted" << std::endl;
    struct epoll_event event, events[1];
    int epoll_fd = epoll_create1(0);

    if(epoll_fd == -1)
    {
        return 4;
    }
    
    event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLRDHUP | EPOLLOUT;
    event.data.u64 = sock;
    uint64_t val = (unsigned long)(14) << 32;
    event.data.u64 |= val;
    setnonblocking(sock);

    std::cout << "val: " << 14 << std::endl;
    std::cout << "sock: " << sock << std::endl;
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll_fd);
        return 5;
    }
    
    std::cout << "Waiting epoll" << std::endl;
    int size = epoll_wait(epoll_fd, events, 1, -1);
    std::cout << "Epoll end" << std::endl;
    for(int i=0;i<size;i++)
    {
        std::cout << "Event: " << events[i].events << std::endl;
        //int len = read(events->data.fd, buf, 1024);
        //std::cout << std::string(buf, len) << std::endl;
        if((events[i].events & EPOLLIN) == EPOLLIN)
        {
            std::cout << "IN" << std::endl;
        }
        if((events[i].events & EPOLLOUT) == EPOLLOUT)
        {
            std::cout << "OUT" << std::endl;
        }
        uint64_t help = events[i].data.u64;
        help = help >> 32;
        std::cout << "value: " << help << std::endl;
        std::cout << "sock: " << int(events[i].data.u64) << std::endl;
    }

    if(close(epoll_fd))
    {
        fprintf(stderr, "Failed to close epoll file descriptor\n");
        return 1;
    }
    close(sock);
    close(listener);
    return 0;
}