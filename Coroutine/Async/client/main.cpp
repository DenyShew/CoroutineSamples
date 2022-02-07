#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

char buf[512];

int main()
{
    char* help = "Master";
    memset(buf, 512, 0);
    memcpy(buf, help, 7);
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(51003);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "bad" << std::endl;
        return 2;
    }

    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<int> dist{5, 13};
    std::this_thread::sleep_for(std::chrono::seconds(dist(e)));

    if(send(sock, buf, sizeof(buf), 0) == -1)
    {
        std::cout << "very bad: " << errno << std::endl;
        return 3;
    }

    int len;
    if((len = recv(sock, buf, sizeof(buf), 0)) == -1)
    {
        std::cout << "very very bad: " << errno << std::endl;
        return 3;
    }
    
    close(sock);

    std::cout << std::string(buf, len) << std::endl;

    return 0;
}