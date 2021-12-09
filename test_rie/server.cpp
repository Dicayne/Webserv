#include "../incs/webserv.hpp"
#include "../incs/Request.hpp"
#include "../incs/AResponse.hpp"
#include "../incs/Resp2.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

#define PORT 8080

/*	This is the generci container sockaddr: it allows the OS tobe able to read the first couple of bytes that identify the address family.
	This structures needs to have three fields filled before using it with bind(): sin_family, sin_port and sin_addr.

	struct sockaddr_in 
	{ 
		__uint8_t		sin_len;		//
		sa_family_t		sin_family;		//	The address family used when creating the socket.
		in_port_t		sin_port;		//	The port number (the transport address) :
											If you're a client and won't be receiving incoming connections, you'll usually
											just let the OS pick any available port number by specifying 0. 
											If you're a server you'll generally pick specific number since clients will
											need to know a port number to connect to.
		struct in_addr	sin_addr;		//	The address for this socket (your machine's IP address (there is one per network
											interface)). Usually we don't care to specify and can let the OS use what it
											wants by giving him this special address : 0.0.0.0, define by INADDR_ANY.
		char			sin_zero[8];	//
	};
*/

int main()
{
	
	int	domain = AF_INET;	// As we want to specify the IP address family.
	int	type = SOCK_STREAM;	// As we want to specify virtual circuit service.
	int	protocol = 0;		// Since there’s only one form of virtual circuit service, there are no variations of the protocol.

	std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");

	int	server_fd = socket(domain, type, protocol);
	if (server_fd == -1)
	{
		std::cerr << RED << "Server error: " << NC << "Socket creation failed.\n";
		return (-1);
	}
	
	struct sockaddr_in		addr;
	socklen_t				addrLen = sizeof(addr);
	
	memset((char *)&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_fd, (struct sockaddr *)&addr, addrLen) == -1)
	{
		std::cerr << RED << "Server error: " << NC << "Socket binding failed.\n";
		std::cerr << std::strerror(errno);
		return (-1);
	}

	int	backlog = 1;

	if (listen(server_fd, backlog) == -1)
	{	
		std::cerr << RED << "Server error: " << NC << "Couldn't listen to socket.\n";
		exit(EXIT_FAILURE);
	}

// While (1)
// {
	std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        
	int	newSocket = accept(server_fd, (struct sockaddr *)&addr, &addrLen);
	if (newSocket == -1)
	{
		std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
		exit(EXIT_FAILURE);
	}

	char buffer[30000] = {0};
	recv(newSocket, buffer, 30000, 0);
    std::cout << RED << buffer << NC << std::endl;
	std::string	buf = buffer;

	Request	firstRequest(buf);
	
	std::ifstream ms;
	ms.open(firstRequest.getUrl());

	if (ms.is_open() == false)
	{
		Resp2	success("HTTP 1.1", "400", "NOPE", "OH SHIT");

		std::cout << PURPLE << success << NC;
		// std::string fuck("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 8\n\nFUCK IT!");
		
		int ret = send(newSocket, success.respond(), 26 , 0);
		// int ret = send(newSocket, fuck.c_str(), fuck.size(), 0);
		std::cout << "RET OF SEND() = " << ret << std::endl;
		// send(newSocket, success.buildResponse(), 8, 0);
		// std::cout << root << ": Not Found\n";
		// ms.open("./html/error/404.html");
	}
	else
	{
		std::cout << GREEN << "TRUE\n" << NC << std::endl;
		// Resp2	success("HTTP 1.1", "200", "OK", "OH \nYEAAAAAAAAAH \nBABY");
		// send(newSocket, success.buildResponse(), 8, 0);
		// // send(newSocket, hello.c_str(), hello.size(), 0);
	}
    std::cout << "---> 'Hi' message sent in response\n";
    close(newSocket);
// } 
	close(server_fd);
    return 0;
}
