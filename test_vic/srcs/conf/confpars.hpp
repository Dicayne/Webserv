/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:55:46 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/08 18:21:26 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARS_HPP
#define CONFPARS_HPP
#include "../../../incs/webserv.hpp"

#include <vector>
#include <map>
#include <string>

#include "serv_block.hpp"

class confpars
{
private:
	std::ifstream	fc;
	std::string		path;
protected:

	bool sendfile;
	std::map<int, std::string> error_page;
	std::vector<serv_block> server;
	bool error_conf;

public:
	confpars();
	~confpars();

	// FD MANIPULATION
	void open_fc(const char *path);
	void reopen_fc();
	void close_fc();
	void read_fc(std::ifstream &fc);

	// GETTER
	std::ifstream &get_fd() {return (this->fc);}

	// PARSING
	void pars_line(std::string line);

	// EXEPTIONS

	class FileNotFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			FileNotFound(std::string path) : _msg(path + " Not Found"){}
			virtual ~FileNotFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
};

#endif