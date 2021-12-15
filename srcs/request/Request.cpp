/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/15 18:21:38 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Request.hpp"

// Request::Request()
// {
// }

Request::Request(std::string& buf) : request(buf)
{
	size_t pos;
	while ((pos = buf.find("\r")) != std::string::npos)
		buf.replace(pos, 1, "\n");

	std::stringstream	b(buf);
	std::string 		line;

// Voir si on fait vraiment ça où si on met pas plutot un vecteur là aussi, car on assume que cette première ligne est la même tout le temps alors que si le client est nous depuis un terminal : non
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

std::string		Request::extractInfo(std::string& line) const
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
std::string		Request::extractMapped(std::string& line) const
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
	std::string	key = this->extractInfo(line);
	std::string	mapped = this->extractMapped(line);

	std::pair<std::string, std::string> elem(key, mapped);
	this->stock.insert(elem);

	return ;
}
/*
	All setters (one for each attribute corresponding to a field of the HTTP request):
	(The following three assign the value of the second element of the pair returned by std::map::find(),
	which corresponds to the mapped-value of the std::map this->info).
*/
void		Request::setMethod(std::string& line){ this->method = this->extractInfo(line); return ; }
void		Request::setUrl(std::string& line){ this->url = this->extractInfo(line); (this->url).insert(0, 1, '.'); return ; }
void		Request::setProtocolVersion(std::string& line){ this->protocol_version = this->extractInfo(line); return ; }
void		Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Host:");
	if (it != this->stock.end())
		this->host = it->second;
	return ;
}
void		Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("User-Agent:");
	if (it != this->stock.end())
		this->user_agent = it->second;
	return ;
}
void		Request::setAccept()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Accept:");
	if (it != this->stock.end())
		this->accept = it->second;
	return ;
}
void		Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Accept-Language:");
	if (it != this->stock.end())
		this->accept_language = it->second;
	return ;
}
void		Request::setAcceptEncoding()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Accept-Encoding:");
	if (it != this->stock.end())
		this->accept_encoding = it->second;
	return ;
}
void		Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Connection:");
	if (it != this->stock.end())
		this->body = it->second;
	return ;
}
void		Request::setBody()
{
	std::map<std::string, std::string>::iterator	it = this->stock.find("Body:");
	if (it != this->stock.end())
		this->body = it->second;
	return ;
}
/*	All getters (one for each attribute corresponding to a field of the HTTP request):	*/
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
