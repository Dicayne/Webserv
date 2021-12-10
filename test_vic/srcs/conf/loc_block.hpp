/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_block.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:20 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/10 18:52:12 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOC_BLOCK_HPP
#define LOC_BLOCK_HPP

#include "../../../incs/webserv.hpp"

#include <vector>
#include <map>

class loc_block
{
private:
	std::vector<std::string>	_block;
	std::string					_path_conf;
	std::string					_path;
	std::string					_root;
	std::vector<std::string>	_method_limit;
	std::vector<std::string>	_index;
	bool						_autoindex;

public:
	loc_block(/* args */);
	~loc_block();

	// PARSING
	void pars_loc(std::vector<std::string> block, std::string path);

	// SETTER
	void set_path(std::string value);
	void set_root(std::string value);
	void set_method_limit(std::string value);
	void set_index(std::string value);
	void set_autoindex(std::string value);

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
	class NoPathFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoPathFound(std::string path) : _msg(" Path is missing in " + path){}
			virtual ~NoPathFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoRootFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoRootFound(std::string path) : _msg(" Root is missing in " + path){}
			virtual ~NoRootFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoIndexFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoIndexFound(std::string path) : _msg(" Index is missing in " + path){}
			virtual ~NoIndexFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
};

#endif
