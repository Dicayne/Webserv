/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/07 19:40:31 by mabriand         ###   ########.fr       */
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

	// std::cout << PURPLE << ret << NC << "  " << std::strerror(errno) << '\n';

	// std::cout << PURPLE << this->_request << NC << '\n';

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
	this->_err_referer = false;
	this->_url_dir = false;
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

	this->set_contentLength();
	this->set_contentType();

	this->setBody(buf);
	this->set_queryString();

	this->treatUrl();

	this->defineProtocolVersion();
	this->defineStatusCode();
	this->defineUrl();

	std::cout << CYAN << "URL Send-> " << this->_response_url << " " << this->_response_status_code <<  NC << '\n';

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
	this->_base_url = this->extractInfo(line);

	if (this->_base_url.size() > 512) // In order to have a small url to pass to provoque the error.
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
		this->_connection = it->second;
	return ;
}
void				Request::setReferer()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Referer:");
	if (it != this->_stock.end())
		this->_referer = this->treat_referer(it->second);
	return ;
}

// void				Request::setBody()
// {
// 	std::map<std::string, std::string>::iterator	it = this->_stock.find("Body:");
// 	if (it != this->_stock.end())
// 		this->_body = it->second;
// 	if (this->_request.size() > (unsigned long)this->_block->get_client_max_body_size() && this->_referer.size() == 0)
// 		this->_response_status_code = 413; // Pour le moment car je ne trouve pas s'il y a un code erreur précis
// 	return ;
// }

void                Request::setBody(std::string& full_resp)
{
    size_t pos = full_resp.find_last_of("\n");
    std::string    tmp(full_resp.substr(pos + 1, full_resp.size() - (pos + 1)));
    this->_body = tmp;
    if (this->_body.size() > (unsigned long)this->_block->get_client_max_body_size()) //&& this->_referer.size() == 0/)
        this->_response_status_code = 413; // Pour le moment car je ne trouve pas s'il y a un code erreur précis
    return ;
}

void				Request::set_queryString()
{
	if (this->_method == "GET")
	{	size_t	i;

		i = this->_base_url.find_first_of('?');
		if (i != std::string::npos)
		{
			this->_queryString.assign(this->_base_url, i + 1, std::string::npos);
			// this->_base_url = this->_path.substr(0, i);
		}
	}
	else if (this->_method == "POST")
	{
		this->_queryString = this->_body;
	}
	return ;
}
void				Request::set_contentLength()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Content-Length:");
	if (it != this->_stock.end())
		this->_content_length = it->second;
	return ;
}
void				Request::set_contentType()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Content-Type:");
	if (it != this->_stock.end())
		this->_content_type = it->second;
	else
		this->_content_type = "";
	return ;
}
void				Request::defineProtocolVersion()
{
	this->_response_protocol_version = this->_protocol_version;
	return ;
}
void				Request::defineStatusCode()
{
	if (this->_method == "DELETE")
	{
		std::cout << YELLOW << "WE GON DELETE" << NC << std::endl;
		struct stat buffer;
    	if (stat(this->_url.c_str(), &buffer) == -1)
        {
			this->_response_status_code = 404;
			return ;
		}
		std::cout << YELLOW << "IT DOES EXIST" << NC << std::endl;
		if (remove (this->_url.c_str()) == 0)
		{
			this->_response_status_code = 200;
		}
		else
		{
			this->_response_status_code = 500;
		}
   	 	
	}
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

std::string			Request::treat_referer(std::string ref)
{
	std::string ret = ref;
	ret.erase(ret.begin(), ret.begin() + 7);

	std::string host = this->_block->get_host();
	if (ret.size() > host.size())
	{
		std::string tmp(ret.begin(), ret.begin() + host.size());
		if (tmp.compare(host) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size() + 1);
	}

	std::string port = std::to_string(this->_block->get_port());
	if (ret.size() > port.size())
	{
		std::string tmp(ret.begin(), ret.begin() + port.size());
		if (tmp.compare(port) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size());
	}
	return (ret);
}

/*	All getters (one for each attribute corresponding to a field of the HTTP _request):
*/
const std::string&	Request::getMethod() const{ return (this->_method); }
const std::string&	Request::getUrl() const{ return (this->_url); }
const std::string&	Request::getBaseUrl() const { return (this->_base_url); }
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
const bool& Request::get_url_dir() const { return (this->_url_dir); }
const std::string&	Request::returnProtocolVersion() const{ return (this->_response_protocol_version); }
const std::string&	Request::returnUrl() const{ return (this->_response_url); }
int					Request::returnStatusCode() const{ return (this->_response_status_code); }

const std::string&			Request::get_queryString() const{ return(this->_queryString); }
const std::string&			Request::get_contentLength() const{ return(this->_content_length); }
const std::string&			Request::get_contentType() const{return(this->_content_type); }

std::ostream&		operator<<(std::ostream& os, const Request& r)
{
	os << "method: [" << r.getMethod() << "]" << std::endl;
	os << "url: [" << r.getUrl() << "]" << std::endl;
	os << "base url: [" << r.getBaseUrl() << "]" << std::endl;
	os << "protocol version :[" << r.getProtocolVersion() << "]" << std::endl;
	os << "host :[" << r.getHost() << "]" << std::endl;
	os << "user agent :[" << r.getUserAgent() << "]" << std::endl;
	os << "accept :[" << r.getAccept() << "]" << std::endl;
	os << "accept language :[" << r.getAcceptLanguage() << "]" << std::endl;
	os << "accept encoding :[" << r.getAcceptEncoding() << "]" << std::endl;
	os << "connection :[" << r.getConnection() << "]" << std::endl;
	os << "referer :[" << r.getReferer() << "]" << std::endl;
	os << "body :[" << r.getBody() << "]" << std::endl;
	os << "query string :[" << r.get_queryString() << "]" << std::endl;
	os << "content length :[" << r.get_contentLength() << "]" << std::endl;
	os << "content type :[" << r.get_contentType() << "]" << std::endl;
	return (os);
}
