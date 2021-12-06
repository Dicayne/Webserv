#include "../incs/color.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string hello("Hello from client");
    char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << RED << "Error=> " << NC << "Socket creation error\n";
        return (-1);
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        std::cout << "\nInvalid address/ Address not supported\n";
        return (-1);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "\nConnection Failed \n";
        return (-1);
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    long valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}