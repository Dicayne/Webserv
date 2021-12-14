/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:35 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/14 19:31:56 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

Server::Server(/* args */)
{
	std::cout << "PASS\n";
}

Server::~Server()
{
}

int Server::server_is_alive = 1;

void Server::Server_launch(confpars *conf)
{
			// int	domain = AF_INET;	// As we want to specify the IP address family.
		// int	type = SOCK_STREAM;	// As we want to specify virtual circuit service.
		// int	protocol = 0;		// Since there’s only one form of virtual circuit service, there are no variations of the protocol.
		int	server_fd;
		int	server_fd2;
		int opt = 1;
		serv_block server = conf->get_server()[0];

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

		// if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
			// std::cout << "Fcntl failed. errno: " << errno << std::endl;
		struct sockaddr_in		addr;

		int addr_size = sizeof(addr);
		memset((char *)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(server.get_port());
		addr.sin_addr.s_addr = inet_addr(server.get_host().c_str());

		if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			std::cerr << RED << "Server error: " << NC << "Socket binding failed.\n";
			std::cerr << std::strerror(errno);
			exit(EXIT_FAILURE);
		}

		int	backlog = 1;

		if (listen(server_fd, backlog) < 0)
		{
			std::cerr << RED << "Server error: " << NC << "Couldn't listen to socket.\n";
			exit(EXIT_FAILURE);
		}
		// while (Server::server_is_alive == 1)
		while (1)
		{
			std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

			// int	newSocket = accept(server_fd, NULL, NULL);
			int	newSocket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addr_size);
			if (newSocket == -1)
			{
				std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
				std::cerr << std::strerror(errno);
				exit(EXIT_FAILURE);
			}

			char buffer[30000] = {0};
			recv(newSocket, buffer, 30000, 0);
			// std::cout << RED << buffer << NC << std::endl;
			std::string	buf = buffer;

			Request	firstRequest(buf);
			buf.clear();

			std::ifstream ms;
			ms.close();
			ms.open(firstRequest.getUrl());
			if (ms.is_open() == false)
			{
				ms.close(); // Because we're goning to open another
				ms.open(server.get_error_page().find("404")->second);
				if (ms.is_open() == false)
					std::cout << "I can't open html\n";
				else
					std::cout << "The html is open!\n";
				ms.close();

				std::string url(server.get_error_page().find("404")->second);
				Resp2	failure("HTTP/1.1", "400", url);
				send(newSocket, failure.respond(), failure.getResponse().size() , 0);
			}
			else
			{
				std::cout << "FOUND\n";
				Resp2	failure("HTTP/1.1", "200", firstRequest.getUrl());
				send(newSocket, failure.respond(), failure.getResponse().size() , 0);
				ms.close();
			}
			std::cout << "---> 'Hi' message sent in response\n";
			close(newSocket);
		}
		close(server_fd);
}