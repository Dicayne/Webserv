/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/10 10:56:51 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Request.hpp"

Request::Request(int socket, serv_block *block)  :  _socket(socket), _block(block)
{
	char buffer[30001] = {0};
	int ret = recv(socket, buffer, 30000, MSG_DONTWAIT);

	std::string	buf = buffer;
	this->_request = buf;
	
	std::cout << PURPLE << ret << NC << "  " << std::strerror(errno) << '\n';
	this->parseBuf(buf);
	buf.clear();
	return ;
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
	this->setBody();

	std::cout << *this << std::endl;

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
void				Request::setUrl(std::string& line)
{
	this->_url = this->extractInfo(line);
	(this->_url).insert(0, 1, '.');
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
	int sub = this->_response_status_code - 200;
	if (sub <= 8 || sub == 10 || sub == 26)
		this->_response_url = this->_url;
	else
	{
		std::stringstream	out;
		std::string			str;
		
		out << this->_response_status_code;
		str = out.str();
		
		std::map<std::string, std::string>				tmp = this->_block->get_error_page();
		std::map<std::string, std::string>::iterator	it = tmp.find(str);
		
		this->_response_url = it->second;
		std::cout << BLUE << this->_response_url << NC << std::endl;
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
const std::string&	Request::getBody() const{ return(this->_body); }

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
	os << "[" << r.getBody() << "]" << std::endl;
	return (os);
}
