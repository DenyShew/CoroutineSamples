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
    int index;
    std::coroutine_handle<task::promise_type> coro;
};

class io_handler
{
friend class tcp_socket;
friend class accept_socket;
private:
    struct elementary_task
    {
        std::coroutine_handle<task::promise_type> coro;
        bool is_set;
    };
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
            std::cout << "current sock event: " << info.sock << std::endl;
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
            if((events[i].events & EPOLLIN) == EPOLLIN)
            {
                std::cout << "epollin" << std::endl;
                if(tasks[info.index].is_set)
                {
                    tasks[info.index].coro.resume();
                }
                continue;
            }
        }
    }
    void set_coro_by_index(int index, std::coroutine_handle<task::promise_type> coro)
    {
        tasks[index].coro = coro;
        tasks[index].is_set = true;
    }
private:
    //return index
    io_task add_sock(int sock)
    {
        io_task rtr;
        rtr.index = tasks.size();
        tasks.resize(tasks.size() + 1);

        socket_info info;
        info.sock = sock;
        info.index = rtr.index;

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLRDHUP | EPOLLOUT;
        ev.data.u64 = pack(info);

        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev))
        {
            throw std::runtime_error("Can't add in epoll");
        }

        return rtr;
    }
private:
    int events_size;
    int epoll_fd;
    struct epoll_event* events;
    std::vector<elementary_task> tasks;
};

template<typename T>
class awaiter;

template<>
class awaiter<void>
{
public:
    awaiter(io_handler& handler, int index, const std::function<void()>& on_resume, bool ready)
        : handler(handler)
        , index(index)
        , on_resume(on_resume)
        , ready(ready)
    {

    }
    void await_suspend(std::coroutine_handle<task::promise_type> coro)noexcept
    {
        handler.set_coro_by_index(index, coro);
    }
    bool await_ready()noexcept
    {
        return ready;
    }
    void await_resume()noexcept
    {
        on_resume();
    }
private:
    io_handler& handler;
    int index;
    std::function<void()> on_resume;
    bool ready;
};

template<typename T>
class awaiter
{
public:
    awaiter(io_handler& handler, int index, const std::function<T()>& on_resume, bool ready)
        : handler(handler)
        , index(index)
        , on_resume(on_resume)
        , ready(ready)
    {

    }
    void await_suspend(std::coroutine_handle<task::promise_type> coro)noexcept
    {
        handler.set_coro_by_index(index, coro);
    }
    bool await_ready()noexcept
    {
        return ready;
    }
    T await_resume()noexcept
    {
        return on_resume();
    }
private:
    io_handler& handler;
    int index;
    std::function<T()> on_resume;
    bool ready;
};

class tcp_socket
{
friend class accept_socket;
public:
    awaiter<std::string> read()
    {
        std::cout << "read: " << tasks.index << std::endl;
        return awaiter<std::string>(handler, tasks.index, std::bind(&tcp_socket::on_read, this), false);
    }
    awaiter<void> write(const std::string& val)
    {
        std::cout << "write: " << tasks.index << std::endl;
        return awaiter<void>(handler, tasks.index, std::bind(&tcp_socket::on_write, this, val), true);
    }
    ~tcp_socket()
    {
        std::cout << "destroy" << std::endl;
    }
private:
    tcp_socket(io_handler& handler, int sock)
        : handler(handler)
        , sock(sock)
    {
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFD, 0) | O_NONBLOCK);
        std::cout << "tcp: " << sock << std::endl;
        tasks = handler.add_sock(sock);
    }
    std::string on_read()
    {
        std::cout << "on_read" << std::endl;
        char buf[512];
        int res = ::read(sock, buf, sizeof(buf));
        std::cout << "~on_read: " << res << " " << errno << std::endl;
        return std::string(buf, res);
    }
    void on_write(std::string str)
    {
        std::cout << "on_write" << std::endl;
        char buf[512];
        std::copy(str.begin(), str.begin() + std::max(str.size(), sizeof(buf)), buf);
        int res = ::write(sock, buf, sizeof(buf));
        std::cout << "~on_write: " << res << std::endl;
    }
private:
    io_task tasks;
    io_handler& handler;
    int sock;
};

class accept_socket
{
public:
    accept_socket(io_handler& handler, int port)
        : handler(handler)
    {
        struct sockaddr_in addr;
        listener = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "listener: " << listener << std::endl;
        if(listener < 0)
        {
            throw std::runtime_error("Can't createe listener");
        }
        
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            throw std::runtime_error("Can't bind the listener");
        }
        if(listen(listener, 1)==-1)
        {
            throw std::runtime_error("Can't start listen");
        }
        io_task current_task = handler.add_sock(listener);
        index = current_task.index;
    }
    awaiter<tcp_socket> accept()
    {
        std::cout << "accept: " << index << std::endl;
        return awaiter<tcp_socket>(handler, index, std::bind(&accept_socket::on_accept, this), false);
    }
private:
    tcp_socket on_accept()
    {
        std::cout << "on_accept" << std::endl;
        return tcp_socket(handler, ::accept(listener, 0, 0));
    }
private:
    io_handler& handler;
    int listener;
    int index;
};

task accepting(accept_socket sock)
{
    while(true)
    {
        tcp_socket tcp_sock = co_await sock.accept();
        std::string res = co_await tcp_sock.read();
        std::cout << "res: " << res << std::endl;
        res = "Hello, " + res;
        co_await tcp_sock.write(res);
    }
}

int main()
{
    try
    {
        io_handler handler(1);
        accept_socket acc(handler, 51003);

        task accept_client = accepting(acc);
        for(;;)
        {
            handler.run();
        }
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}