/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:24:59 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/06 16:55:09 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response.hpp"

Response::Response(const Request &req, serv_block *block) : _req(req), _block(block)
{
	this->_target_dir = this->_req.get_url_dir();

	// Independant from the parameters
	this->setMimeMap();
	this->setMessagesMap();

	// Dependant from the parameters
	this->setProtocolVersion(this->_req.returnProtocolVersion());
	this->setStatus(this->_req.returnStatusCode());
	this->setStatusMessage(this->_req.returnStatusCode());
	if (this->_req.returnStatusCode() == 300 )
		this->setLocation(this->_req.returnUrl());
	this->setDate();
	this->setServer();
	this->setConnection();
	this->setKeepAlive();
	this->setBody(this->_req.returnUrl());
	this->setContentType(this->_req.returnUrl());
	this->setContentLenght();

	this->buildResponse();

}
Response::~Response(){}

void				Response::setProtocolVersion(const std::string& protocol_version)
{
	this->_protocol_version = protocol_version + " ";

	std::pair<std::string, std::string> elem("Protocol-Version", this->_protocol_version);
	this->_stock.insert(elem);

	return ;
}
void				Response::setStatus(int status)
{
	std::stringstream	out;

	out << status;
	this->_status = out.str() + " ";

	std::pair<std::string, std::string> elem("Status", this->_status);
	this->_stock.insert(elem);

	return ;
}
void				Response::setStatusMessage(int status)
{
	std::map<int, std::string>::iterator	itf = (this->_messages).find(status);
	this->_status_message = itf->second;
	std::pair<std::string, std::string> elem("Status-Message", this->_status_message);
	this->_stock.insert(elem);

	return ;
}
void				Response::setLocation(std::string url)
{
	std::string location(url.begin(), url.begin() + url.find_last_of('/'));

	std::cout << RED << location << NC << '\n';
	std::pair<std::string, std::string> elem("Location", location);
	this->_stock.insert(elem);

	return ;
}
void				Response::setDate()
{
    std::time_t result = std::time(nullptr);
	std::string date(std::asctime(std::localtime(&result)));
	date.pop_back();

	this->_date = date;
	std::pair<std::string, std::string> elem("Date", this->_date);
	this->_stock.insert(elem);

	return ;
}
void				Response::setServer()
{
	this->_server = this->_block->get_server_name();

	std::pair<std::string, std::string> elem("Server", this->_server);
	this->_stock.insert(elem);

	return ;
}
void				Response::setConnection()
{
	this->_connection = "keep-alive";

	std::pair<std::string, std::string> elem("Connection", this->_connection);
	this->_stock.insert(elem);

	return ;
}
void				Response::setKeepAlive()
{
	this->_keep_alive = "timeout=3";

	std::pair<std::string, std::string> elem("Keep-Alive", this->_keep_alive);
	this->_stock.insert(elem);

	return ;
}
void				Response::setContentType(const std::string& url)
{
	size_t pos = url.find_last_of(".", url.size());
	if (pos == std::string::npos)
		return ;

	std::string key(url.substr(pos, url.size() - pos));
	std::map<std::string, std::string>::iterator	itf = (this->_mime).find(key);
	this->_content_type = itf->second;
	std::pair<std::string, std::string> elem("Content-Type", this->_content_type);
	this->_stock.insert(elem);
	this->_selected_mime = key;
	return ;
}

void				Response::setContentLenght()
{
	std::stringstream content_length;
	content_length << this->_body.size();

	this->_content_length = content_length.str();
	std::pair<std::string, std::string> elem("Content-Length", this->_content_length);
	this->_stock.insert(elem);

	return ;
}

bool IsPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void				Response::setBody(const std::string& url)
{
	std::string tmp_url = this->_req.getUrl();
	std::string url_dir(tmp_url.begin(), tmp_url.begin() + tmp_url.find_last_of('/'));
	bool path_exist = IsPathExist(url_dir);

	std::cout << "URL_DIR: " << url_dir << '\n';
	if (url_dir == ".")
		path_exist = false;

	if (this->_block->get_autoindex() && this->_req.returnStatusCode() == 404 && this->_target_dir && path_exist)
	{
		std::string test = build_autoindex_body(url_dir.c_str());
		for (size_t i = 0; i < test.size(); i++)
		{
			this->_body.push_back(test[i]);
		}
	}
	else
	{
		std::ifstream ms(url.c_str(), std::ios::in | std::ios::binary);

		if (!ms.eof() && !ms.fail())
		{
			ms.seekg(0, std::ios_base::end);
			std::streampos fileSize = ms.tellg();
			this->_body.resize(fileSize);

			ms.seekg(0, std::ios_base::beg);
			ms.read(&this->_body[0], fileSize);
		}

		ms.close();
	}
	return ;
}

const std::string&	Response::getProtocolVersion() const{ return (this->_protocol_version); }
const std::string&	Response::getStatus() const{ return (this->_status); }
const std::string&	Response::getStatusMessage() const{ return (this->_status_message); }
const std::string&	Response::getDate() const{ return (this->_date); }
const std::string&	Response::getServer() const{ return (this->_server); }
const std::string&	Response::getContentType() const{ return (this->_content_type); }
const std::string&	Response::getContentLenght() const{ return (this->_content_length); }
const std::string&  Response::getMime() const{ return (this->_selected_mime); }
//
const std::vector< char >& Response::getVecResponse() const {return (this->_response); }

void				Response::buildMime(const std::string& key, const std::string& mapped)
{
	std::pair<std::string, std::string>	elem(key, mapped);
	this->_mime.insert(elem);
	return ;
}


void				Response::buildMessages(int key, const std::string& mapped)
{
	std::pair<int, std::string>	elem(key, mapped);
	this->_messages.insert(elem);
	return ;
}


void				Response::fill_resp(const std::string& line)
{
	this->_response.insert(this->_response.end(), line.begin(), line.end());
}

void				Response::buildPartResp(const std::string& key)
{
	std::map<std::string, std::string>::iterator	itf = (this->_stock).find(key);

	if (key.compare("Protocol-Version") == 0 || key.compare("Status") == 0)
		fill_resp(itf->second);
	else if (key.compare("Location") == 0)
		fill_resp(itf->first + ": " + itf->second + "\r\n");
	else if (key.compare("Status-Message") == 0)
		fill_resp(itf->second + "\r\n");
	else
		fill_resp(itf->first + ": " + itf->second + "\r\n");
	return ;
}
void				Response::buildResponse()
{
	this->buildPartResp("Protocol-Version");
	this->buildPartResp("Status");
	this->buildPartResp("Status-Message");
	if (this->_status.compare("300 ") == 0)
		this->buildPartResp("Location");
	this->buildPartResp("Date");
	this->buildPartResp("Server");
	this->buildPartResp("Connection");
	this->buildPartResp("Keep-Alive");
	this->buildPartResp("Content-Type");
	this->buildPartResp("Content-Length");

	this->fill_resp("\r\n");
	this->_response.insert(this->_response.end(), this->_body.begin(), this->_body.end());

	return ;
}
std::ostream&		operator<<(std::ostream& os, const Response& r)
{
	os << "[" << r.getProtocolVersion() << "]" << std::endl;
	os << "[" << r.getStatus() << "]" << std::endl;
	os << "[" << r.getStatusMessage() << "]" << std::endl;
	os << "[" << r.getDate() << "]" << std::endl;
	os << "[" << r.getServer() << "]" << std::endl;
	os << "[" << r.getContentType() << "]" << std::endl;
	os << "[" << r.getContentLenght() << "]" << std::endl;
	return (os);
}
