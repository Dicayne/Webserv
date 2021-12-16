/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:35 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/16 19:07:54 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

int Server::server_is_alive = 1;

Server::Server(/* args */)
{

}

Server::~Server()
{
}

void Server::Server_closeSocket(int socket)
{
	if (socket >= 0)
		close(socket);
}

void Server::Server_closeAllSocket()
{
	for (std::map< int, sockaddr_in >::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		this->Server_closeSocket(it->first);
	std::cout << YELLOW << "All opened Sockets have been closed.\n" << NC;
}

void Server::Server_setSocket()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		std::cout << this->_servers[i].get_host() << '\n';
		int	mysock;
		int opt = 1;

		if ((mysock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError("Socket creation error");
		}
		if (fcntl(mysock, F_SETFL, O_NONBLOCK) < 0)
			std::cout << "Fcntl failed. errno: " << errno << std::endl;

		if (setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError("Setsockopt \"SO_REUSEADDR\" error");
		}
		if (setsockopt(mysock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError("Setsockopt \"SO_REUSEPORT\" error");
		}

		struct sockaddr_in		myaddr;

		int addr_size = sizeof(myaddr);
		memset((char *)&myaddr, 0, addr_size);
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons(this->_servers[i].get_port());
		myaddr.sin_addr.s_addr = inet_addr(this->_servers[i].get_host().c_str());

		if (bind(mysock, (struct sockaddr *)&myaddr, addr_size) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError(std::strerror(errno));
		}

		int	backlog = 1;

		if (listen(mysock, backlog) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError("Couldn't listen to socket.");
		}
		this->_socket.insert(std::make_pair(mysock, myaddr));
	}
}

void Server::Server_setFd()
{
	FD_ZERO(&this->_all_sock);
	for (std::map< int, sockaddr_in >::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		FD_SET(it->first, &this->_all_sock);
	this->_nfds = this->_socket.rbegin()->first;
}


void Server::Server_init(confpars *html, std::vector< serv_block > serv)
{
	this->_html = html;
	this->_servers = serv;
}

void Server::Server_launch(confpars *conf)
{
		serv_block server = conf->get_server()[0];

		Server_setSocket();
		Server_setFd();


		while (1)
		{
			this->_readfds = this->_all_sock;
			this->_writefds = this->_all_sock;

			// std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
			int ret_select = select(this->_nfds + 1, &this->_readfds, &this->_writefds, NULL, NULL);

			if (ret_select < 0 && (errno != EINTR))
				throw ServerError(std::strerror(errno));
			for (std::map< int, sockaddr_in >::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
			{
				if (FD_ISSET(it->first, &this->_readfds))
				{
					std::cout << it->first << '\n';
					int	clientSocket = accept(it->first, NULL, NULL);
					if (clientSocket < 0 && errno != EAGAIN)
					{
						std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
						std::cerr << std::strerror(errno);
						exit(EXIT_FAILURE);
					}
					std::cout << YELLOW << "New connexion from: " << clientSocket << NC << '\n';
					FD_SET(clientSocket, &this->_all_sock);

					this->_client_fd.push_back(clientSocket);
					if (clientSocket > this->_nfds)
						this->_nfds = clientSocket;
				}
			}
			for (std::vector<int>::iterator it = this->_client_fd.begin(); it != this->_client_fd.end(); it++)
			{
				if (FD_ISSET(*it, &this->_readfds))
				{
					
					// char buffer[30001] = {0};
					// int ret = recv(*it, buffer, 30000, MSG_DONTWAIT);

					// std::string	buf = buffer;
					// std::cout << PURPLE << ret << NC << "  " << std::strerror(errno) << '\n';

					std::map<std::string, std::string> error_page = server.get_error_page();
					
					//Request	firstRequest(buf, &error_page);
					Request	firstRequest(*it, &error_page);
					// buf.clear();

					Response r(firstRequest.returnProtocolVersion(), firstRequest.returnStatusCode(), firstRequest.returnUrl());
					send(*it, r.respond(), r.getResponse().size() , 0);
				}
				// FD_CLR(*it, &this->_all_sock);
				// close(*it);
			}
		}

		// while (Server::server_is_alive == 1)
		// while (1)
		// {
		// 	std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

		// 	// int	newSocket = accept(this->_socket, NULL, NULL);
		// 	int	newSocket = accept(this->_socket, (struct sockaddr *)&addr, (socklen_t*)&addr_size);
		// 	if (newSocket == -1)
		// 	{
		// 		std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
		// 		std::cerr << std::strerror(errno);
		// 		exit(EXIT_FAILURE);
		// 	}

		// 	char buffer[30000] = {0};
		// 	recv(newSocket, buffer, 30000, 0);
		// 	// std::cout << RED << buffer << NC << std::endl;
		// 	std::string	buf = buffer;

		// 	Request	firstRequest(buf);
		// 	buf.clear();

		// 	std::ifstream ms;
		// 	ms.close();
		// 	ms.open(firstRequest.getUrl());
		// 	if (ms.is_open() == false)
		// 	{
		// 		ms.close(); // Because we're goning to open another
		// 		ms.open(server.get_error_page().find("404")->second);
		// 		if (ms.is_open() == false)
		// 			std::cout << "I can't open html\n";
		// 		else
		// 			std::cout << "The html is open!\n";
		// 		ms.close();

		// 		std::string url(server.get_error_page().find("404")->second);
		// 		Resp2	failure("HTTP/1.1", "400", url);
		// 		send(newSocket, failure.respond(), failure.getResponse().size() , 0);
		// 	}
		// 	else
		// 	{
		// 		std::cout << "FOUND\n";
		// 		Resp2	failure("HTTP/1.1", "200", firstRequest.getUrl());
		// 		send(newSocket, failure.respond(), failure.getResponse().size() , 0);
		// 		ms.close();
		// 	}
		// 	std::cout << "---> 'Hi' message sent in response\n";
		// 	close(newSocket);
		// }
		// close(this->_socket);
}
