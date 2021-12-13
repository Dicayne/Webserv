/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:52:54 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/13 16:28:20 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "conf/confpars.hpp"
#include "request/Request.hpp"
#include "response/AResponse.hpp"
#include "response/Resp2.hpp"

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
	confpars conf;

	try
	{
		conf.open_fc(CONF_DEFAULT_PATH);
		conf.pars_fc(conf.get_fd());
		conf.close_fc();

		serv_block server = conf.get_server()[0];

		int	domain = AF_INET;	// As we want to specify the IP address family.
		int	type = SOCK_STREAM;	// As we want to specify virtual circuit service.
		int	protocol = 0;		// Since there’s only one form of virtual circuit service, there are no variations of the protocol.
		int	server_fd;
		int opt = 1;

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

		struct sockaddr_in		addr;
		socklen_t				addrLen = sizeof(addr);

		memset((char *)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(server.get_port());
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
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

	while (1)
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
	catch(const std::exception &e)
	{
		std::cerr << RED << "ERROR: " << NC << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}