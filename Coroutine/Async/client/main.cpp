#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

char buf[1024];

int main()
{
    char* help = "Hello";
    memset(buf, 1024, 0);
    memcpy(buf, help, 5);
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(34250); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "bad" << std::endl;
        return 2;
    }

    /*if(recv(sock, buf, sizeof(buf), 0) == -1)
    {
        std::cout << "very bad" << std::endl;
        return 3;
    }*/
    
    close(sock);

    return 0;
}