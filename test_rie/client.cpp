#include "../incs/color.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

#define PORT 8080

int main()
{
    int                 sock = 0;
    struct sockaddr_in  serv_addr;
    std::string         hello("Hello from client");
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << RED << "Client error: " << NC << "Socket creation failed.\n";
        return (-1);
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << RED << "Client error: " << NC << "Invalid address/Address not supported.\n";
        return (-1);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << RED << "Client error: " << NC << "Connection failed.\n";
        return (-1);
    }
    
    char                buffer[1024] = {0};
    send(sock, hello.c_str(), hello.size(), 0);
    std::cout << "The 'Hello' message has been sent.\n";
    recv(sock, buffer, 1024, 0);
    std::cout << buffer << std::endl;
    
    return (0);
}