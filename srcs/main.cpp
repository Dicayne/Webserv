/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:52:54 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/15 12:16:14 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "conf/confpars.hpp"
#include "request/Request.hpp"
#include "response/AResponse.hpp"
#include "response/Resp2.hpp"
#include "server/Server.hpp"

void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		Server::server_is_alive = 0;
	}
	if (signum == SIGPIPE)
	{
		std::cout << "SP caught" << std::endl;
	}
}

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
	confpars *conf = new confpars();
	// if (signal(SIGINT, signal_handler) == SIG_ERR)
	// 	return (1);
	// if (signal(SIGPIPE, signal_handler) == SIG_ERR)
	// 	return (1);

	try
	{
		conf->open_fc(CONF_DEFAULT_PATH);
		conf->pars_fc(conf->get_fd());
		conf->close_fc();

		Server *serv = new Server();

		serv->Server_init(conf, conf->get_server());
		serv->Server_launch(conf);
		delete conf;
		delete serv;
	}
	catch(const std::exception &e)
	{
		std::cerr << RED << "ERROR: " << NC << e.what() << '\n';
		delete conf;
		return (EXIT_FAILURE);
	}
	std::cout << "\n\nSSSHHHHHHUUUUUUUUUUUUUUUUUUUUU\n\n";
	return (EXIT_SUCCESS);
}
