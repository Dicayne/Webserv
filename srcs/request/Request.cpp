/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/28 09:18:11 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Request.hpp"

Request::Request(int socket, serv_block *block)  :  _socket(socket), _block(block)
{
	this->_request_ready = false;
}

Request::~Request(){}

std::string			Request::extractInfo(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (std::isspace(*ite) == 0 && ite != line.end())
		ite++;
	std::string	info(it, ite);
	while (std::isspace(*ite) != 0 && ite != line.end())
		ite++;

	line.erase(it, ite);
	return (info);
}

int					Request::parse()
{
	char buffer[65537] = {0};
	int ret = recv(this->_socket, buffer, 65536, MSG_DONTWAIT);

	if (ret <= 0)
		return (ret);
	std::string	buf = buffer;
	this->_request = buf;

	std::cout << PURPLE << ret << NC << "  " << std::strerror(errno) << '\n';
	this->parseBuf(buf);
	buf.clear();
	this->_request_ready = true;
	return (ret);
}

std::string			Request::extractMapped(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (ite != line.end())
		ite++;
	std::string	info(it, ite);

	line.erase(it, ite);
	return (info);
}
void				Request::buildMap(std::string& line)
{
	std::string	key = this->extractInfo(line);
	std::string	mapped = this->extractMapped(line);

	std::pair<std::string, std::string> elem(key, mapped);
	this->_stock.insert(elem);

	return ;
}
void				Request::parseBuf(std::string& buf)
{
	this->_response_status_code = 0;

	size_t pos;
	while ((pos = buf.find("\r")) != std::string::npos)
		buf.replace(pos, 1, "\n");

	std::stringstream	b(buf);
	std::string 		line;

	std::getline(b, line);
	this->setMethod(line);
	this->setUrl(line);
	this->setProtocolVersion(line);

	while (std::getline(b, line))
		this->buildMap(line);

	this->setHost();
	this->setUserAgent();
	this->setAccept();
	this->setAcceptLanguage();
	this->setAcceptEncoding();
	this->setConnection();
	this->setReferer();
	this->setBody();

	std::cout << *this << std::endl;

	std::cout << GREEN << this->_url << NC << '\n';
	this->treatUrl();
	std::cout << BLUE << this->_url << NC << '\n';

	this->defineProtocolVersion();
	this->defineStatusCode();
	this->defineUrl();

	return ;
}
/*	All setters (one for each attribute corresponding to a field of the HTTP _request):
	(The following three assign the value of the second element of the pair returned by std::map::find(),
	which corresponds to the mapped-value of the std::map this->info).
*/
void				Request::setMethod(std::string& line)
{
	this->_method = this->extractInfo(line);
	if (this->_method.compare("GET") != 0 && this->_method.compare("POST") != 0 && this->_method.compare("DELETE"))
		_response_status_code = 405;
	return ;
}

loc_block			*Request::find_loc_block(std::string url)
{
	std::vector< loc_block > tmp_loc = this->_block->get_location();
	std::string tmp_url = url;

	for (std::vector< loc_block >::iterator it = tmp_loc.begin(); it != tmp_loc.end(); it++)
	{
		if (url[url.size() - 1] == '/')
			tmp_url.pop_back();
		if (tmp_url == it->get_path())
			return(&(*it));
	}
	return (NULL);
}

std::string			Request::treat_referer(std::string ref)
{
	std::string ret = ref;
	ret.erase(ret.begin(), ret.begin() + 7);

	if (ret.size() > this->_block->get_host().size())
	{
		std::string tmp(ret.begin(), ret.begin() + this->_block->get_host().size());
		if (tmp.compare(this->_block->get_host()) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size() + 1);
	}

	std::string port = std::to_string(this->_block->get_port());

	if (ret.size() > port.size())
	{
		std::string tmp(ret.begin(), ret.begin() + port.size());
		if (tmp.compare(port) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size());
	}
	loc_block *loc = find_loc_block(ret);
	if (loc != NULL)
	{
		if (this->_url.size() > ret.size())
		{
			std::string tmp(this->_url.begin(), this->_url.begin() + ret.size());
			if (ret == tmp)
				this->_url.erase(this->_url.begin(), this->_url.begin() + tmp.size() - 1);
		}
		ret = "/" + loc->get_root();
		this->_loc_referer = true;
	}
	return (ret);
}

void				Request::treatUrl()
{
	std::vector< loc_block > tmp = this->_block->get_location();
	std::string tmp_url = this->_url;
	bool changed = false;

	if (_loc_referer == false)
	{
		for (std::vector< loc_block >::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			if (tmp_url.size() > it->get_path().size())
			{
				std::string tmp(tmp_url.begin(), tmp_url.begin() + it->get_path().size());
				if (tmp == it->get_path())
				{
					std::cout << CYAN << this->_url << NC << '\n';
					this->_url.erase(this->_url.begin(), this->_url.begin() + tmp.size());
					this->_url.insert(0, "/" + it->get_root());
					std::cout << CYAN << this->_url << NC << '\n';
					changed = true;
					break;
				}
			}
			if (tmp_url[tmp_url.size() - 1] == '/')
				tmp_url.pop_back();
			if (tmp_url == it->get_path())
			{
				this->_url = "/" + it->get_root() + "/" + it->get_index()[0];
				changed = true;
				break;
			}
		}
		if (changed == false)
		{
			tmp_url.clear();
			tmp_url = "/" + this->_block->get_default_root() + "/";
			if (this->_url.compare("/") != 0)
				tmp_url += this->_url;
			else
				tmp_url += this->_block->get_default_path();
			this->_url.clear();
			this->_url = tmp_url;
		}
	}
	else
	{
		this->_url = this->_referer + tmp_url;
	}
	this->_url.insert(0, 1, '.');
}

void				Request::setUrl(std::string& line)
{
	this->_url = this->extractInfo(line);

	if (this->_url.size() > 512) // In order to have a small url to pass to provoque the error.
		this->_response_status_code = 414;
	return ;
}
void				Request::setProtocolVersion(std::string& line)
{
	this->_protocol_version = this->extractInfo(line);
	return ;
}
void				Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Host:");
	if (it != this->_stock.end())
		this->_host = it->second;
	return ;
}
void				Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("User-Agent:");
	if (it != this->_stock.end())
		this->_user_agent = it->second;
	return ;
}
void				Request::setAccept()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept:");
	if (it != this->_stock.end())
		this->_accept = it->second;
	return ;
}
void				Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept-Language:");
	if (it != this->_stock.end())
		this->_accept_language = it->second;
	return ;
}
void				Request::setAcceptEncoding()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept-Encoding:");
	if (it != this->_stock.end())
		this->_accept_encoding = it->second;
	return ;
}
void				Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Connection:");
	if (it != this->_stock.end())
		this->_body = it->second;
	return ;
}
void				Request::setReferer()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Referer:");
	if (it != this->_stock.end())
		this->_referer = this->treat_referer(it->second);
	return ;
}
void				Request::setBody()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Body:");
	if (it != this->_stock.end())
		this->_body = it->second;
	if (this->_body.size() > (unsigned long)this->_block->get_client_max_body_size())
		this->_response_status_code = 400; // Pour le moment car je ne trouve pas s'il y a un code erreur précis
	return ;
}
void				Request::defineProtocolVersion()
{
	this->_response_protocol_version = this->_protocol_version;
	return ;
}
void				Request::defineStatusCode()
{
	if (this->_response_status_code != 0)
		return ;

	std::ifstream ms;

	ms.close();
	ms.open(this->_url);
	if (ms.is_open() == true)
		this->_response_status_code = 200;
	else
		this->_response_status_code = 404;
	ms.close();

	return ;
}

void		Request::defineUrl()
{
	if (this->_response_status_code < 400)
		this->_response_url = this->_url;
	else
	{
		std::stringstream	out;
		std::string			str;

		out << this->_response_status_code;
		str = out.str();

		std::map<std::string, std::string> error_page_stocked = this->_block->get_error_page();
		std::map<std::string, std::string>::iterator	it = error_page_stocked.find(str);

		if (it != error_page_stocked.end())
			this->_response_url = it->second;
		else if (str[0] == '4')
			this->_response_url = error_page_stocked.find("4xx")->second;
		else if (str[0] == '5')
			this->_response_url = error_page_stocked.find("5xx")->second;
	}
	return ;
}
/*	All getters (one for each attribute corresponding to a field of the HTTP _request):
*/
const std::string&	Request::getMethod() const{ return (this->_method); }
const std::string&	Request::getUrl() const{ return (this->_url); }
const std::string&	Request::getProtocolVersion() const{ return (this->_protocol_version); }
const std::string&	Request::getHost() const{ return(this->_host); }
const std::string&	Request::getUserAgent() const{ return(this->_user_agent); }
const std::string&	Request::getAccept() const{ return(this->_accept); }
const std::string&	Request::getAcceptLanguage() const{ return(this->_accept_language); }
const std::string&	Request::getAcceptEncoding() const{ return (this->_accept_encoding); }
const std::string&	Request::getConnection() const{ return(this->_connection); }
const std::string&	Request::getReferer() const{ return(this->_referer); }
const std::string&	Request::getBody() const{ return(this->_body); }
serv_block*	Request::getBlock() { return(this->_block); }
const bool&	Request::is_request_ready() const { return (this->_request_ready); }

const std::string&	Request::returnProtocolVersion() const{ return (this->_response_protocol_version); }
const std::string&	Request::returnUrl() const{ return (this->_response_url); }
int					Request::returnStatusCode() const{ return (this->_response_status_code); }

std::ostream&		operator<<(std::ostream& os, const Request& r)
{
	os << "[" << r.getMethod() << "]" << std::endl;
	os << "[" << r.getUrl() << "]" << std::endl;
	os << "[" << r.getProtocolVersion() << "]" << std::endl;
	os << "[" << r.getHost() << "]" << std::endl;
	os << "[" << r.getUserAgent() << "]" << std::endl;
	os << "[" << r.getAccept() << "]" << std::endl;
	os << "[" << r.getAcceptLanguage() << "]" << std::endl;
	os << "[" << r.getAcceptEncoding() << "]" << std::endl;
	os << "[" << r.getConnection() << "]" << std::endl;
	os << "[" << r.getReferer() << "]" << std::endl;
	os << "[" << r.getBody() << "]" << std::endl;
	return (os);
}
