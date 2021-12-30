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

int setnonblocking(int sock)
{
    int opts;

    opts = fcntl(sock,F_GETFL);
    if (opts < 0)
    {
    perror("fcntl(F_GETFL)");
    return -1;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock,F_SETFL,opts) < 0)
    {
    perror("fcntl(F_SETFL)");
    return -1;
    }

    return 0;
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
    struct epoll_event event, events[2];
    int epoll_fd = epoll_create1(0);

    if(epoll_fd == -1)
    {
        return 4;
    }
    
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    event.data.fd = sock;
    setnonblocking(sock);
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll_fd);
        return 5;
    }
    
    std::cout << "Waiting epoll" << std::endl;
    int size = epoll_wait(epoll_fd, events, 2, -1);
    std::cout << "Epoll end" << std::endl;
    for(int i=0;i<size;i++)
    {
        std::cout << "Event: " << events[i].events << std::endl;
        int len = read(events->data.fd, buf, 1024);
        std::cout << std::string(buf, len) << std::endl;
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