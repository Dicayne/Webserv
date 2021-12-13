/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pars_serv.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:56:18 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/13 12:58:16 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/webserv.hpp"
#include "conf/confpars.hpp"

int main()
{
	confpars conf;

	try
	{
		conf.open_fc(CONF_DEFAULT_PATH);
		conf.pars_fc(conf.get_fd());
		conf.close_fc();
		serv_block serv1 = conf.get_server()[0];
		serv_block serv2 = conf.get_server()[1];
		std::cout << serv1.get_host() << '\n';
		std::cout << serv2.get_host() << '\n';
	}
	catch(const std::exception &e)
	{
		std::cerr << RED << "ERROR: " << NC << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
