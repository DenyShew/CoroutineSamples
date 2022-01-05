#include <coroutine>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <iostream>

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
        void return_void(){}
        void unhandled_exception()
        {
            
        }
    };
public:
    task(std::coroutine_handle<promise_type> coro)
    {

    }
};

class io_handler
{
public:

};

class awaiter
{
public:

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
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFD, 0)|O_NONBLOCK);
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