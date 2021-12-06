#include "../incs/color.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

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
	int server_fd;
	int domain;
	int type;
	int protocol;
	long valread;
	char *hello = "Hello from server";
	
	domain = AF_INET; // As we want to specify the IP address family.
	type = SOCK_STREAM; // As we want to specify virtual circuit service.
	protocol = 0; // Since there’s only one form of virtual circuit service, there are no variations of the protocol.

	//char *err_msg;

	server_fd = socket(domain, type, protocol);
	if (server_fd == -1)
	{
		//err = "Error: unable to create socket";
		//write(1, err, ft_strlen(err));
		return (-1);
	}
	
	struct sockaddr_in		*addr;
	addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	if (!addr)
		return (-1);
	socklen_t			addrLen;

	const int 			PORT = 8080;
	addrLen = sizeof(*addr);
	//memset((char *)&addr, 0, sizeof(addr)); 
	addr->sin_family = AF_INET;
	addr->sin_port = htons(PORT);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);// ????????

	int	retBind;
	retBind = bind(server_fd, (struct sockaddr *)addr, addrLen);
	if (retBind == -1)
	{
		return (-1);
	}

	int	retListen;
	int	backlog;
	backlog = 1;
	retListen = listen(server_fd, backlog);
	if (retListen == -1)
	{	
		exit(EXIT_FAILURE); // pk pas return(-1) ?
	}
	
	 while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        
		int	newSocket;
		newSocket = accept(server_fd, (struct sockaddr *)addr, &addrLen);
		if (newSocket == -1)
		{
			exit(EXIT_FAILURE); // pk pas return(-1) ?
		}
		char buffer[30000] = {0};
        valread = read( newSocket , buffer, 30000);
        printf("%s\n",buffer );
        write(newSocket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(newSocket);
	}
    return 0;
}
