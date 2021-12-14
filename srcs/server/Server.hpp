/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:32 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/14 17:33:56 by vmoreau          ###   ########.fr       */
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
	/* data */
public:
	static int server_is_alive;
	Server(/* args */);
	~Server();
	void Server_launch(confpars *conf);
};

#endif