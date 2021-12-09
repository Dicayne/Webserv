/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:55:46 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/09 18:48:30 by vmoreau          ###   ########.fr       */
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
	std::ifstream				_fc;
	std::string					_path;
	std::vector<std::string>	_file;
	std::vector< std::vector<std::string> >	_server_block;

protected:

	bool						_sendfile;
	std::vector<serv_block>		_server;
	std::map<std::string, std::string>		_error_page;

public:
	confpars();
	~confpars();

	// FD MANIPULATION
	void open_fc(const char *path);
	void reopen_fc();
	void close_fc();

	// GETTER
	std::ifstream &get_fd() {return (this->_fc);}

	// SETTER
	void set_sendfile(std::string value);
	void set_error_page(std::string value);

	// PARSING
	void pars_fc(std::ifstream &fc);
	void pars_http();

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
	class ConfFile : public std::exception
	{
		private:
			std::string _msg;
		public:
			ConfFile(std::string msg) : _msg(msg){}
			virtual ~ConfFile() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoServerFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoServerFound(std::string path) : _msg(" No Server block found in " + path){}
			virtual ~NoServerFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};

	// Private Function
	private:
		std::vector<std::string> separate_server_block(std::vector<std::string> tmp, size_t nb_serv);
		std::vector<std::string> save_server_block(std::vector<std::string> tmp, size_t *pos);
};

#endif
