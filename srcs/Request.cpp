/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/07 14:30:03 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"

Request::Request(std::string& buf) : request(buf)
{
	this->setMethod(buf);
	this->setUrl(buf);
	this->setProtocolVersion(buf);
	
	this->buildMap(buf);

	// this->setHost();
	// this->setUserAgent(buf);
	// this->setAccept(buf);
	// this->setAcceptLanguage(buf);
	// this->setAcceptCharset(buf);
	// this->setConnections(buf);
	// this->setBody(buf);
	std::cout << *this << std::endl;
}

Request::~Request(){}

std::string		Request::getInfo(std::string& buf, char sep) const
{
	size_t	pos = buf.find(sep);
	std::string::iterator	it = buf.begin();
	std::string::iterator	ite = it;
	
	for (size_t i = 0; i < pos; i++)
		ite++;

	std::string	info(it, ite);
	
	// std::string::iterator	i = buf.begin();
	while (std::isprint(buf[*ite]) != 0 && std::isspace(buf[*ite]) != 0)
		ite++;
	buf.erase(it, ite + 1);
	return (info);
}

void			Request::buildMap(std::string& buf)
{
	while (buf.size() > 0)
	{
		std::string	key = this->getInfo(buf, ' ');
		std::string	mapped = this->getInfo(buf, '\n');

		std::pair<std::string, std::string> elem(key, mapped);

		this->info.insert(elem);
	}
	return ;
}

// std::string		Request::getHeader(std::string& buf, std::string key) const
// {
// 	std::string head = this->getInfo(buf, ' ');
// 	if (head.compare(key) != 0)
// 	{	
// 		//return (NULL);
// 	}
// 	return (this->getInfo(buf, '\n'));
// }

void		Request::setMethod(std::string& buf){ this->method = this->getInfo(buf, ' '); return ; }
void		Request::setUrl(std::string& buf){ this->url = this->getInfo(buf, ' '); return ; } // rajouter un .
void		Request::setProtocolVersion(std::string& buf){ this->protocol_version = this->getInfo(buf, '\n'); return ; }

// void		Request::setHost()
// {
// 	std::map<std::string, std::string>::iterator	it = this->info.find("Host");
// 	// std::pair<std::string, std::string>				elem = *it;

// 	// std::cout << elem.first << std::endl;
// 	// std::cout << elem.second << std::endl;

// 	this->host = it->second;
// 	return ;
// }
// void		Request::setUserAgent(std::string& buf){ this->user_agent = this->getHeader(buf, "User"); return ; }
// void		Request::setAccept(std::string& buf){ this->accept = this->getHeader(buf, "Accept"); return ; }
// void		Request::setAcceptLanguage(std::string& buf){ this->accept_language = this->getHeader(buf, "Accept"); return ; }
// void		Request::setAcceptCharset(std::string& buf){ this->accept_charset = this->getHeader(buf, "Accept"); return ; }
// void		Request::setConnections(std::string& buf){ this->connections = this->getHeader(buf, "Accept"); return ; }
// void		Request::setBody(std::string& buf){ this->body = this->getHeader(buf, "Accept"); return ; }

const std::string&		Request::getMethod() const{ return (this->method); }
const std::string&		Request::getUrl() const{ return (this->url); }
const std::string&		Request::getProtocolVersion() const{ return (this->protocol_version); }
// const std::string&		Request::getHost() const{ return(this->host); }
// const std::string&		Request::getUserAgent() const{ return(this->user_agent); }
// const std::string&		Request::getAccept() const{ return(this->accept); }
// const std::string&		Request::getAcceptLanguage() const{ return(this->accept_language); }
// const std::string&		Request::getAcceptCharset() const{ return (this->accept_charset); }
// const std::string&		Request::getConnections() const{ return(this->connections); }
// const std::string&		Request::getBody() const{ return(this->body); }

std::ostream&	operator<<(std::ostream& os, const Request& r)
{
	os << "Method:\t\t\t" << r.getMethod() << std::endl;
	os << "URL:\t\t\t" << r.getUrl() << std::endl;
	os << "Protocol version:\t" << r.getProtocolVersion() << std::endl;
	// os << "Host:\t\t\t" << r.getHost() << std::endl;
	// os << "User agent:\t\t" << r.getUserAgent() << std::endl;
	// os << "Accept:\t\t\t" << r.getAccept() << std::endl;
	// os << "Accept language:\t\t" << r.getAcceptLanguage() << std::endl;
	// os << "Accept charset:\t\t" << r.getAcceptCharset() << std::endl;
	// os << "Connections:\t\t" << r.getConnections() << std::endl;
	// os << "Body:\t\t\t" << r.getBody() << std::endl;
	return (os);
}