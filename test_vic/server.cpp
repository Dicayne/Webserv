/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 14:08:02 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/13 14:48:53 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/color.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>

#define PORT 80
void clear_buf(char buff[1024])
{
	int i = 0;

	while (i < 1024)
	{
		buff[i] = '\0';
		i++;
	}
}

int main()
{
	int server_fd, new_sock;
	struct sockaddr_in address;
	int opt = 1;
	int addr_size = sizeof(address);
	char buffer[10024] = {0};
	std::string msg, recep;
	std::ifstream ms;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Socket creation error\n";
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Setsockopt error\n";
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Setsockopt error\n";
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Bind failed\n";
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "listen\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Server setup on port " << PORT << " and wating for client connect:\n\n";
	while (1)
	{
		// *****************RECEIVED*****************
		if ((new_sock = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addr_size)) < 0)
		{
			std::cerr << RED << "Error=> " << NC << "accepte\n";
			exit(EXIT_FAILURE);
		}
		std::cout << GREEN << "CONNECTED\n" << NC;
		clear_buf(buffer);
		recv(new_sock, buffer, 10024, 0);
		recep = buffer;
		std::cout << YELLOW << "Client:\n" << NC << recep;


		// *******************SEND*******************
		msg = "HTTP/1.1 200 OK\r\nConnection: keep_alive\r\nContent-type: text/html\r\n\r\n";
		std::string root(recep.begin() + 3, recep.begin() + (recep.find_first_of('\n') - 10));
		root[0] = '.';
		recep.clear();
		ms.close();
		ms.open(root.c_str());
		if (ms.is_open() == false)
		{
			std::cout << root << ": Not Found\n";
			ms.open("./html/error/404.html");
		}
		else
			std::cout << "File Found\n";
		std::string url("./html/error/404.html");
		// while(std::getline(ms, recep))
		// {
		// 	msg += recep;
		// 	msg += '\n';
		// }
		ms.close();
		if (send(new_sock, msg.c_str(), msg.size() , 0) < 0)
		{
			std::cerr << RED << "Error=> " << NC << "Message not send\n";
			exit(EXIT_FAILURE);
		}
		close(new_sock);
	}
	return (0);
}