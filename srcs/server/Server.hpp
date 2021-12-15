/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:32 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/15 19:02:56 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../incs/webserv.hpp"
#include "../conf/confpars.hpp"
#include "../request/Request.hpp"
#include "../response/AResponse.hpp"
#include "../response/Resp2.hpp"

class Server
{
private:

	fd_set							_all_sock;
	fd_set							_readfds;
	fd_set							_writefds;
	int								_nfds;
	std::vector< serv_block >		_servers;
	confpars 						*_html;
	std::map< int, sockaddr_in >	_socket;
	std::vector< int >				_client_fd;

	void Server_setSocket();
	void Server_setFd();
	void Server_closeSocket(int socket);
	void Server_closeAllSocket();
public:
	static int server_is_alive;
	Server(/* args */);
	~Server();

	void Server_init(confpars *html, std::vector< serv_block > serv);
	void Server_launch(confpars *conf);

	class ServerError : public std::exception
	{
		private:
			std::string _msg;
		public:
			ServerError(std::string msg) : _msg(msg){}
			virtual ~ServerError() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
};

#endif
