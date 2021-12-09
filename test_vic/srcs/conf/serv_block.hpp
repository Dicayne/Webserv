/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_block.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:09 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/09 19:19:23 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_BLOCK_HPP
#define SERV_BLOCK_HPP

#include "../../../incs/webserv.hpp"

#include <vector>
#include <map>

#include "loc_block.hpp"
// #include "confpars.hpp"

class serv_block
{
private:
	std::vector<std::string> _block;

protected:
	int						_port;
	std::string				_server_name;
	int						_client_max_body_size;
	std::vector<loc_block>	_location;
	// confpars				_tmp;

public:
	serv_block();
	~serv_block();

	// PARSING
	void pars_serv(std::vector<std::string> block);
};

#endif
