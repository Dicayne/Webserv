/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 16:02:13 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/08 18:42:06 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confpars.hpp"
#include <wctype.h>

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

confpars::confpars()
{
	// std::cout << "Default constructor call\n";
}

confpars::~confpars()
{
}

void confpars::open_fc(const char *path)
{
	this->path = path;
	this->fc.open(this->path.c_str());
	if (this->fc.is_open() == false)
		throw FileNotFound(this->path);
}

void confpars::reopen_fc()
{
	this->fc.close();
	this->open_fc(this->path.c_str());
}

void confpars::close_fc()
{
	this->fc.close();
}

void confpars::read_fc(std::ifstream &fc)
{
	std::string line;
	while(std::getline(fc, line))
	{
		if (!is_emptyline_commentline(line))
			this->pars_line(norm_line(line));
	}
	this->reopen_fc();
}

void confpars::pars_line(std::string line)
{

}