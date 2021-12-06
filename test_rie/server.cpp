#include "../incs/color.hpp"
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

	std::string hello("Hi from server");

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
		return (-1);
	}

	int	backlog = 1;

	if (listen(server_fd, backlog) == -1)
	{	
		std::cerr << RED << "Server error: " << NC << "Couldn't listen to socket.\n";
		exit(EXIT_FAILURE);
	}
	
	while(1)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        
		int	newSocket = accept(server_fd, (struct sockaddr *)&addr, &addrLen);
		if (newSocket == -1)
		{
			std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		recv(newSocket, buffer, 30000, 0);
    	std::cout << buffer << std::endl;
		send(newSocket, hello.c_str(), hello.size(), 0);
        std::cout << "---> 'Hi' message sent in response\n";
        close(newSocket);
	}
    return 0;
}
