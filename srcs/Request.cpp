/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/08 15:06:37 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"

Request::Request(std::string& buf) : request(buf)
{
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
	this->setBody();
	
	std::cout << *this << std::endl;
	return ;
}

Request::~Request(){}

std::string		Request::getInfo(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	// isspace() returns:	!= 0 if IT IS a whitespace (true)
	//						== 0 if IT'S NOT a whitespace (false)
	while (std::isspace(*ite) == 0 && ite != line.end())
		ite++;
	std::string	info(it, ite);
	while (std::isspace(*ite) != 0 && ite != line.end())
		ite++;
	line.erase(it, ite);

	return (info);
}
std::string		Request::getMapped(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (ite != line.end())
		ite++;
	std::string	info(it, ite);
	
	line.erase(it, ite);
	return (info);
}
void			Request::buildMap(std::string& line)
{
	std::string	key = this->getInfo(line);
	std::string	mapped = this->getMapped(line);

	std::pair<std::string, std::string> elem(key, mapped);		
	this->info.insert(elem);
	
	return ;
}
// The following setters assign the value of the second element of the pair returned by std::map::find(), which corresponds to the mapped-value of the std::map this->info
void		Request::setMethod(std::string& line){ this->method = this->getInfo(line); return ; }
void		Request::setUrl(std::string& line){ this->url = this->getInfo(line); (this->url).insert(0, 1, '.'); return ; }
void		Request::setProtocolVersion(std::string& line){ this->protocol_version = this->getInfo(line); return ; }
void		Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Host:");
	if (it != this->info.end())
		this->host = it->second;
	return ;
}
void		Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("User-Agent:");
	if (it != this->info.end())
		this->user_agent = it->second;
	return ;
}
void		Request::setAccept()
{ 
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept:");
	if (it != this->info.end())
		this->accept = it->second;
	return ;
}
void		Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Language:");
	if (it != this->info.end())
		this->accept_language = it->second;
	return ;
}
void		Request::setAcceptEncoding()
{ 
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Encoding:");
	if (it != this->info.end())
		this->accept_encoding = it->second;
	return ;
}
void		Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Connection:");
	if (it != this->info.end())
		this->body = it->second;
	return ;
}
void		Request::setBody()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Body:");
	if (it != this->info.end())
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