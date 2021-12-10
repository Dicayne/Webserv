/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_block.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:09 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/10 16:04:22 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_BLOCK_HPP
#define SERV_BLOCK_HPP

#include "../../../incs/webserv.hpp"

#include <vector>
#include <map>

#include "loc_block.hpp"
// #include "confpars.hpp"

#define BODY_DEFAULT_SIZE 100000

class serv_block
{
private:
	std::string				_path;
	std::vector<std::string> _block;
	std::vector< std::vector<std::string> >	_loc_block;

protected:
	int						_port;
	std::string				_host;
	std::string				_server_name;
	int						_client_max_body_size;
	std::vector<loc_block>	_location;

public:
	serv_block();
	~serv_block();

	// PARSING
	void pars_serv(std::vector<std::string> block, std::string path);

	// SETTER
	void set_port_host(std::string value);
	void set_server_name(std::string value);
	void set_client_max_body_size(std::string value);
	// EXCEPTION
	class ConfFile : public std::exception
	{
		private:
			std::string _msg;
		public:
			ConfFile(std::string msg) : _msg(msg){}
			virtual ~ConfFile() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoLocationFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoLocationFound(std::string path) : _msg(" No Location block found in " + path){}
			virtual ~NoLocationFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
// Private Function
private:
	std::vector<std::string> save_loc_block(std::vector<std::string> tmp, size_t *pos);
	std::vector<std::string> separate_loc_block(std::vector<std::string> tmp, size_t nb_loc);
	void check_host(std::string value);
};

#endif
