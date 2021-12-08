/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/08 13:11:09 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"

std::string get_line(std::string buf)
{
	int i = 0;
	int j = buf.find_first_of('\r');
	
	while (i < j)
	{
		std::cout << *(buf.begin() + i);
		i++;
	}
	std::cout << "|\n";
	std::string ret(buf.begin(), buf.begin() + buf.find_first_of('\r'));

	std::cout << "1" << ret << "2" << '\n';
	return ret;
}

Request::Request(std::string& buf) : request(buf)
{
	std::stringstream	b(buf);
	std::string 		line;

	get_line(buf);
	std::getline(b, line);
	this->setMethod(line);
	std::cout << PURPLE << line << NC << '\n';
	this->setUrl(line);
	std::cout << PURPLE << line << NC << '\n';
	this->setProtocolVersion(line);
	
	while (std::getline(b, line))
	{	
		std::cout << BLUE << line << NC << std::endl;
		this->buildMap(line);
	}

	this->setHost();
	this->setUserAgent();
	this->setAccept();
	this->setAcceptLanguage();
	this->setAcceptEncoding();
	this->setConnection();
	// this->setBody();
	// std::cout << *this << std::endl;
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
std::string		Request::getEndLine(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (ite != line.end())
		ite++;
	std::string	info(it, ite);
	line.erase(it, ite);
	return (info);
}

// std::string		Request::getKey(std::string& buf) const
// {
// 	std::string::iterator	it = buf.begin();
// 	std::string::iterator	ite = it;

// 	while (std::isspace(*ite) == 0) //| std::iswspace(*ite) == 0)
// 		ite++;
// 	std::string	info(it, ite);
// 	std::cout << info << std::endl;
// 	while (std::isspace(*ite) != 0) //|| std::iswspace(*ite) != 0)
// 		ite++;

// 	buf.erase(it, ite);
// 	return (info);
// }

// std::string		Request::getMapped(std::string& buf) const
// {
// 	std::string::iterator	it = buf.begin();
// 	std::string::iterator	ite = it;

// 	// isspace() returns:	!= 0 if IT IS a whitespace (true)
// 	//						== 0 if IT'S NOT a whitespace (false)
// 	while ((std::isspace(*ite) == 0) || (std::isspace(*ite) == 0 && *ite != ' '))// std::iswspace(*ite) == 0
// 		ite++;
// 	std::string	info(it, ite);
// 	std::cout << GREEN << info << NC << std::endl;

// 	while (std::isspace(*ite) != 0)
// 		ite++;
	
// 	buf.erase(it, ite);
// 	return (info);
// }

void			Request::buildMap(std::string& line)
{
	std::string	key = this->getInfo(line);
	std::cout << key << std::endl;
	std::string	mapped = this->getEndLine(line);
	std::cout << mapped << std::endl;

	std::pair<std::string, std::string> elem(key, mapped);		
	this->info.insert(elem);

	// std::cout << YELLOW << elem.first << NC << std::endl;
	// std::cout << YELLOW << elem.second << NC << std::endl;
	
	return ;
}

void		Request::setMethod(std::string& line)
{
	this->method = this->getInfo(line);
	return ;
}
void		Request::setUrl(std::string& line)
{
	this->url = this->getInfo(line);
	(this->url).insert(0, 1, '.');
	return ; 
}
void		Request::setProtocolVersion(std::string& line)
{
	this->protocol_version = this->getInfo(line);
	return ;
}
void		Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Host:");
	std::pair<std::string, std::string>				elem(*it);
	this->host = elem.second;
	
	std::cout << GREEN << "|" << this->info["Host:"] << "8\n" << NC;

	// std::cout << YELLOW << "[" << elem.first << "]" << NC << std::endl;
	// std::cout << YELLOW << "abcdefg[" << elem.second <<  "]123456789" << NC << std::endl;
	return ;
}
void		Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("User-Agent:");
	std::pair<std::string, std::string>				elem(*it);
	this->user_agent = elem.second;
	return ;
}
void		Request::setAccept()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept = elem.second;
	return ;
}
void		Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Language:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept_language = elem.second;
	return ;
}
void		Request::setAcceptEncoding()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Accept-Encoding:");
	std::pair<std::string, std::string>				elem(*it);
	this->accept_encoding = elem.second;
	return ;
}
void		Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->info.find("Connection:");
	std::pair<std::string, std::string>				elem(*it);
	this->connection = elem.second;
	return ;
}
// void		Request::setBody()
// {
// 	std::map<std::string, std::string>::iterator	it = this->info.find("Body:");
// 	std::pair<std::string, std::string>				elem(*it);
// 	this->body = it->second;
// 	return ;
// }

const std::string&		Request::getMethod() const{ return (this->method); }
const std::string&		Request::getUrl() const{ return (this->url); }
const std::string&		Request::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&		Request::getHost() const{ return(this->host); }
const std::string&		Request::getUserAgent() const{ return(this->user_agent); }
const std::string&		Request::getAccept() const{ return(this->accept); }
const std::string&		Request::getAcceptLanguage() const{ return(this->accept_language); }
const std::string&		Request::getAcceptEncoding() const{ return (this->accept_encoding); }
const std::string&		Request::getConnection() const{ return(this->connection); }
// const std::string&		Request::getBody() const{ return(this->body); }

std::ostream&	operator<<(std::ostream& os, const Request& r)
{
	os << "[" << r.getMethod() << "]" << std::endl;
	os << "[" << r.getUrl() << "]" << std::endl;
	os << "[" << r.getProtocolVersion() << "]" << std::endl;
	os << "[" << r.getHost() << "]" << std::endl;
	// os << "[" << "WESH" << "]" << std::endl;
	os << "[" << r.getUserAgent() << "]" << std::endl;
	os << "[" << r.getAccept() << "]" << std::endl;
	os << "[" << r.getAcceptLanguage() << "]" << std::endl;
	os << "[" << r.getAcceptEncoding() << "]" << std::endl;
	os << "[" << r.getConnection() << "]" << std::endl;
	// os << "[" << r.getBody() << "]" << std::endl;
	return (os);
}