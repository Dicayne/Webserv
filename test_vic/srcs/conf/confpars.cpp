/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 16:02:13 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/09 18:33:47 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confpars.hpp"
#include <wctype.h>

// ********************** UTILS ********************** //

std::string norm_line(std::string line)
{
	std::string ret(line);
	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '\t')
			ret[i] = ' ';
	}
	int j = ret.find_first_not_of(' ');
	std::string ret2(ret.begin() + j, ret.end());
	return (ret2);
}

bool is_emptyline_commentline(std::string line)
{
	int i = 0;

	while (iswspace(line[i]))
		i++;
	if (line[i] == '\0' || line[i] == '#')
		return (true);
	else
		return (false);
}

// ******************* CONSTRUCTOR ******************* //

confpars::confpars() : _sendfile(false)
{
	// std::cout << "Default constructor call\n";
}

confpars::~confpars()
{
}

// ******************** FD MANIP ********************* //
void confpars::open_fc(const char *path)
{
	if (path != NULL)
		this->_path = path;
	this->_fc.open(this->_path.c_str());
	if (this->_fc.is_open() == false)
		throw FileNotFound(this->_path);
}

void confpars::reopen_fc()
{
	this->_fc.close();
	this->open_fc(NULL);
}

void confpars::close_fc()
{
	this->_fc.close();
}


// ********************* SETTER ********************** //

void confpars::set_sendfile(std::string value)
{
	std::string val(value.begin() + value.find_first_not_of(" "), value.end());

	for (std::string::reverse_iterator rit = val.rbegin(); rit != val.rend(); rit++)
	{
		if (*rit != ' ' && *rit != ';')
			break;
		val.pop_back();
	}

	if (val.compare("on") == 0)
		this->_sendfile = true;
	else if (val.compare("off") == 0)
		this->_sendfile = false;
	else
		throw ConfFile(" Sendfile option is \"on\" or \"off\"");
}

void confpars::set_error_page(std::string value)
{
	std::string code_err(value.begin() + value.find_first_not_of(" "), value.begin() + value.find_first_not_of(" ") + 4);
	std::string path_err_tmp(value.begin() + code_err.size(), value.end());
	std::string path_err(path_err_tmp.begin() + path_err_tmp.find_first_not_of(" "), path_err_tmp.end());

	if (code_err.back() != ' ')
		throw ConfFile(" Error_page code is based on 5xx or 4xx only");
	if (code_err.front() != '4' && code_err.front() != '5')
		throw ConfFile(" Error_page code is based on 5xx or 4xx only");
	code_err.pop_back();

	for (size_t i = 0; i < code_err.size(); i++)
	{
		if (!isdigit(code_err[i]))
			if (code_err[i] != 'x')
				throw ConfFile(" CodeError is not digit");
	}
	for (std::string::reverse_iterator rit = path_err.rbegin(); rit != path_err.rend(); rit++)
	{
		if (*rit != ' ' && *rit != ';')
			break;
		path_err.pop_back();
	}

	this->_error_page.insert(std::make_pair(code_err, path_err));
}

// ******************** PARSING ********************** //

void confpars::pars_fc(std::ifstream &fc)
{
	std::string line;

	while(std::getline(fc, line))
	{
		if (!is_emptyline_commentline(line))
			this->_file.push_back(norm_line(line));
	}
	this->reopen_fc();
	this->pars_http();
	for (size_t i = 0; i < this->_server_block.size(); i++)
	{
		serv_block tmp;

		tmp.pars_serv(this->_server_block[i]);
		this->_server.push_back(tmp);
	}

}


void confpars::pars_http()
{
	std::vector<std::string> tmp(this->_file);
	size_t i = 0;
	size_t nb_serv = 0;

	for (size_t j = 0; j < this->_file.size(); j++)
	{
		if (this->_file[j].compare("server") == 0)
			nb_serv++;
	}
	if (nb_serv == 0)
		throw NoServerFound(this->_path);
	if (tmp[i++].compare("http") != 0)
		throw ConfFile(this->_path + " Must start by http block\n");
	if (tmp[i++].compare("{") != 0)
		throw ConfFile(this->_path + " HTTP block \"{\" Missing\n");
	tmp = this->separate_server_block(tmp, nb_serv);

	while (i < tmp.size() - 1)
	{
		std::string key(tmp[i].begin(), tmp[i].begin() + tmp[i].find_first_of(' '));
		std::string value(tmp[i].begin() + key.size(), tmp[i].end());

		if (key.compare("sendfile") == 0)
			this->set_sendfile(value);
		else if (key.compare("error_page") == 0)
			this->set_error_page(value);
		i++;
	}

	// DEBUG //
	std::cout << std::boolalpha << this->_sendfile << '\n';
	for (std::map<std::string, std::string>::iterator it = this->_error_page.begin(); it != this->_error_page.end(); it++)
		std::cout << it->first << "-> \"" << it->second << "\"\n";
}

// ************** INTERNE FUNCTION PARS ************** //

std::vector<std::string> confpars::save_server_block(std::vector<std::string> tmp, size_t *pos)
{
	size_t i = *pos;
	std::vector<std::string> ret;
	int bracket = 0;

	ret.push_back(tmp[i++]);
	if (tmp[i].compare("{") != 0)
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");
	bracket++;
	ret.push_back(tmp[i++]);
	while (i < tmp.size() && bracket != 0)
	{
		if (tmp[i].compare("server") == 0)
			throw ConfFile(this->_path + " Server block \"}\" Missing\n");
		if (tmp[i].compare("{") == 0)
			bracket++;
		if (tmp[i].compare("}") == 0)
			bracket--;
		ret.push_back(tmp[i]);
		i++;
	}
	if (i != tmp.size() && (tmp[i].compare("server") != 0 && tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");
	*pos = i;
	return (ret);
}

std::vector<std::string> confpars::separate_server_block(std::vector<std::string> tmp, size_t nb_serv)
{
	std::vector<std::string>ret;

	size_t i = 0;

	while (nb_serv > 0)
	{
		while (i < tmp.size() && tmp[i].compare("server"))
		{
			ret.push_back(tmp[i]);
			i++;
		}
		this->_server_block.push_back(this->save_server_block(tmp , &i));
		nb_serv--;
	}
	if (i == tmp.size() || (tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " HTTP or last Server block \"}\" Missing\n");
	ret.push_back(tmp[i++]);
	if (i != tmp.size())
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");

	return (ret);
}
