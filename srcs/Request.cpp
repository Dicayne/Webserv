/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/07 19:12:55 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"

Request::Request(std::string& buf) : request(buf)
{
	this->setMethod(buf);
	this->setUrl(buf);
	this->setProtocolVersion(buf);
	
	this->buildMap(buf);

	this->setHost();
	this->setUserAgent();
	this->setAccept();
	this->setAcceptLanguage();
	this->setAcceptEncoding();
	this->setConnection();
	this->setBody();
	// std::cout << *this << std::endl;
	return ;
}

Request::~Request(){}

std::string		Request::getInfo(std::string& buf) const
{
	std::string::iterator	it = buf.begin();
	std::string::iterator	ite = it;

	while (std::isspace(*ite) == 0) //| std::iswspace(*ite) == 0)
		ite++;
	std::string	info(it, ite);
	while (std::isspace(*ite) != 0) //|| std::iswspace(*ite) != 0)
		ite++;

	buf.erase(it, ite);
	return (info);
}

std::string		Request::getKey(std::string& buf) const
{
	std::string::iterator	it = buf.begin();
	std::string::iterator	ite = it;

	while (std::isspace(*ite) == 0) //| std::iswspace(*ite) == 0)
		ite++;
	std::string	info(it, ite);
	std::cout << info << std::endl;
	while (std::isspace(*ite) != 0) //|| std::iswspace(*ite) != 0)
		ite++;

	buf.erase(it, ite);
	return (info);
}

std::string		Request::getMapped(std::string& buf) const
{
	std::string::iterator	it = buf.begin();
	std::string::iterator	ite = it;

	// isspace() returns:	!= 0 if IT IS a whitespace (true)
	//						== 0 if IT'S NOT a whitespace (false)
	while ((std::isspace(*ite) == 0) || (std::isspace(*ite) == 0 && *ite != ' '))// std::iswspace(*ite) == 0
		ite++;
	std::string	info(it, ite);
	std::cout << GREEN << info << NC << std::endl;

	while (std::isspace(*ite) != 0)
		ite++;
	
	buf.erase(it, ite);
	return (info);
}


void			Request::buildMap(std::string& buf)
{
	while (buf.size() > 0)
	{
		std::string	key = this->getInfo(buf);
		std::string	mapped = this->getInfo(buf);

		// std::string	key = this->getKey(buf);
		// std::string	mapped = this->getMapped(buf);
		std::pair<std::string, std::string> elem(key, mapped);		
		this->info.insert(elem);
	}
	return ;
}

void		Request::setMethod(std::string& buf)
{
	this->method = this->getInfo(buf);
	// this->method = this->getKey(buf);
	return ;
}
void		Request::setUrl(std::string& buf)
{
	this->url = this->getInfo(buf);
	// this->method = this->getKey(buf);
	(this->url).insert(0, 1, '.');
	return ; 
}
void		Request::setProtocolVersion(std::string& buf)
{
	this->protocol_version = this->getInfo(buf);
	// this->method = this->getMapped(buf);
	return ;
}
void		Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Host:");
	std::pair<std::string, std::string>				elem(*it);
	this->host = it->second;
	return ;
}
void		Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("User-Agent:");
	std::pair<std::string, std::string>				elem(*it);
	this->user_agent = it->second;
	return ;
}
void		Request::setAccept()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept = it->second;
	return ;
}
void		Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Language:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept_language = it->second;
	return ;
}
void		Request::setAcceptEncoding()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Encoding:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept_encoding = it->second;
	return ;
}
void		Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Connection:");
	std::pair<std::string, std::string>				elem(*it);
	this->connection = it->second;
	return ;
}
void		Request::setBody()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Body:");
	std::pair<std::string, std::string>				elem(*it);
	this->body = it->second;
	return ;
}

const std::string&		Request::getMethod() const{ return (this->method); }
const std::string&		Request::getUrl() const{ return (this->url); }
const std::string&		Request::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&		Request::getHost() const{ return(this->host); }
const std::string&		Request::getUserAgent() const{ return(this->user_agent); }
const std::string&		Request::getAccept() const{ return(this->accept); }
const std::string&		Request::getAcceptLanguage() const{ return(this->accept_language); }
const std::string&		Request::getAcceptEncoding() const{ return (this->accept_encoding); }
const std::string&		Request::getConnection() const{ return(this->connection); }
const std::string&		Request::getBody() const{ return(this->body); }

std::ostream&	operator<<(std::ostream& os, const Request& r)
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
	os << "[" << r.getBody() << "]" << std::endl;
	return (os);
}