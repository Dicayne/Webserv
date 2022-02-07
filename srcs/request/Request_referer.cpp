/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_referer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:50:40 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/05 17:25:06 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

bool IsRefPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

bool		Request::is_referer_autoindex(std::string url)
{
	std::string url_dir(url.begin(), url.begin() + url.find_last_of('/'));
	bool path_exist = IsRefPathExist(url_dir);

	if (url_dir == ".")
		path_exist = false;

	if (this->_block->get_autoindex() && path_exist)
		return (true);
	return (false);
}

bool		Request::is_referer_error(std::string *ref_code)
{
	std::string test_url = ".";
	bool url_dir = is_url_dir(this->_referer);

	if (this->_referer == "/")
		test_url += this->treat_void_url();
	else if (url_dir == true)
		test_url += this->treat_dir_url(this->_referer);
	else
		test_url += this->treat_full_url(this->_referer);

	// std::cout << test_url << '\n';
	std::ifstream ms;

	ms.close();
	ms.open(test_url);
	if (ms.is_open() == true)
		*ref_code = "200";
	else
		*ref_code = "404";
	ms.close();

	if (*ref_code == "404" && url_dir == true)
	{
		if (is_referer_autoindex(test_url) == true)
			*ref_code = "200";
	}
	return (*ref_code == "404" ? true : false);
}

void		Request::treatUrl_with_referer()
{
	// std::cout << RED << "Referer-> " << this->_referer << NC << "\n\n";
	std::string ref_code;
	bool err_ref = is_referer_error(&ref_code);

	if (err_ref)
		this->treatUrl_with_err_referer(ref_code);
	else if (this->_referer == "/")
		this->treatUrl_with_void_referer();
	else if (is_url_dir(this->_referer) == true)
	{
		this->treatUrl_with_dir_referer();
	}
	else
	{
		this->treatUrl_with_full_referer();
	}
}

void		Request::treatUrl_with_void_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string ret;

	for (std::vector<loc_block>::iterator it = loc.begin(); it != loc.end(); it++)
	{
		if (it->get_path() == "/")
		{
			std::string tmp(this->_base_url.begin() + it->get_path().size(), this->_base_url.end());
			ret += "/" + it->get_root() + tmp;

			if (this->is_url_dir(this->_base_url) == true)
				ret += "/" +  it->get_index();
			this->_url += ret;
			return ;
		}
	}
	ret += "/" + this->_block->get_default_root() + this->_base_url;
	if (this->is_url_dir(this->_base_url) == true)
		ret += "/" + this->_block->get_default_path();

	this->_url += ret;
	return;
}

void		Request::treatUrl_with_dir_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::vector<loc_block>::iterator it = location_found(this->_referer, &loc);
	std::vector<loc_block>::iterator it2 = location_found(this->_base_url, &loc);
	std::string ret;

	if (it != loc.end())
	{
		// std::cout << "PASS REF DIR IF\n";

		std::string tmp;
		if (it2 != loc.end())
			tmp.assign(this->_base_url.begin() + it2->get_path().size(), this->_base_url.end());
		else
			tmp = this->_base_url;
		ret += "/" + it->get_root() + tmp;
		if (this->is_url_dir(this->_base_url) == true)
			ret += "/" +  it->get_index();
	}
	else
	{
		// std::cout << "PASS REF DIR ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + this->_base_url;
		if (this->is_url_dir(this->_base_url) == true)
			ret += "/" + this->_block->get_default_path();
	}
	this->_url += ret;
}

void		Request::treatUrl_with_full_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::vector<loc_block>::iterator it = location_found(this->_referer, &loc);
	std::vector<loc_block>::iterator it2 = location_found(this->_base_url, &loc);
	std::string ret;

	if (it != loc.end())
	{
		// std::cout << "PASS REF FULL IF\n";

		std::string tmp;
		if (it2 != loc.end())
			tmp.assign(this->_base_url.begin() + it2->get_path().size(), this->_base_url.end());
		else
			tmp = this->_base_url;
		ret += "/" + it->get_root() + tmp;
	}
	else
	{
		// std::cout << "PASS REF FULL ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + this->_base_url;
	}
	this->_url += ret;
}

void		Request::treatUrl_with_err_referer(const std::string ref_code)
{
	// std::cout << "ERR REFERER PROCESSING !\n";

	std::map<std::string, std::string> error_page_stocked = this->_block->get_error_page();
	std::map<std::string, std::string>::iterator	it = error_page_stocked.find(ref_code);

	std::string error_page;

	if (it != error_page_stocked.end())
		error_page = it->second;
	else if (ref_code[0] == '4')
		error_page = error_page_stocked.find("4xx")->second;
	else if (ref_code[0] == '5')
		error_page = error_page_stocked.find("5xx")->second;

	std::string path(error_page.begin(), error_page.begin() + error_page.find_last_of('/'));
	std::string file(this->_base_url.begin() + this->_base_url.find_last_of('/'), this->_base_url.end());

	this->_url = path + file;

}
