/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:24:59 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/29 22:33:45 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response.hpp"
#include "../request/Request.hpp"

Response::Response(Request *current_request) : _block(current_request->getBlock())
{
	// Independant from the parameters
	this->setMimeMap();
	this->setMessagesMap();

	// Dependant from the parameters
	this->setProtocolVersion(current_request->returnProtocolVersion());
	this->setStatus(current_request->returnStatusCode());
	this->setStatusMessage(current_request->returnStatusCode());
	if (current_request->returnStatusCode() == 300 )
		this->setLocation(current_request->returnUrl());
	this->setDate();
	this->setServer();
	this->setBody(current_request->returnUrl());
	this->setContentType(current_request->returnUrl());
	this->setContentLenght();

	this->buildResponse();
	return ;

}
// Response::Response(const std::string& protocol_version, int status, const std::string& url, serv_block *block) : _block(block)
// {
// 	// Independant from the parameters
// 	this->setMimeMap();
// 	this->setMessagesMap();

// 	// Dependant from the parameters
// 	this->setProtocolVersion(protocol_version);
// 	this->setStatus(status);
// 	this->setStatusMessage(status);
// 	if (status == 300 )
// 		this->setLocation(url);
// 	this->setDate();
// 	this->setServer();
// 	this->setBody(url);
// 	this->setContentType(url);
// 	this->setContentLenght();

// 	this->buildResponse();
// 	return ;

// }
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
void				Response::setBody(const std::string& url)
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
	return ;
}

void				Response::set_cgiBody(std::vector< char > cgi_body)
{
	// size_t i = 0;
	// while (i < this->_body.size())
	// {
	// 	std::cout << GREEN << this->_body[i] << NC;
	// 	++i;
	// }
	// i = 0;
	// while (i < cgi_body.size())
	// {
	// 	std::cout << RED << cgi_body[i] << NC;
	// 	++i;
	// }
	// return ;

	
	// this->_body.erase(it_begin, ite_end);
	// this->_body = cgi_body;
	
	std::vector<char>	header;
	std::vector<char>	newBody;

	size_t i = 0;
	while (cgi_body[i] != '<')
	{
		header.push_back(cgi_body[i]);
		++i;
	}
	while (i < cgi_body.size())
	{
		newBody.push_back(cgi_body[i]);
		++i;
	}

	this->_body = newBody;	


	// i = 0;
	// std::cout << "STOCKED AS HEADER: \n\n" << NC;	
	// while (i < header.size())
	// {
	// 	std::cout << RED << header[i] << NC;
	// 	++i;	
	// }
	// i = 0;
	// std::cout << "STOCKED AS BODY: \n\n" << NC;	
	// while (i < newBody.size())
	// {
	// 	std::cout << RED << newBody[i] << NC;	
	// 	++i;
	// }

	// this->_cgiResp = true;
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

void				Response::setMimeMap()
{
	// a
	this->buildMime(".aac", "audio/aac");
	this->buildMime(".abw", "application/x-abiword");
	this->buildMime(".arc", "application/octet-stream");
	this->buildMime(".avi", "video/x-msvideo");
	this->buildMime(".azw", "application/vnd.amazon.ebook");
	// b
	this->buildMime(".bin", "application/octet-stream");
	this->buildMime(".bmp", "image/bmp");
	this->buildMime(".bz", "application/x-bzip");
	this->buildMime(".bz2", "application/x-bzip2");
	// c
	this->buildMime(".csh", "application/x-csh");
	this->buildMime(".css", "text/css");
	this->buildMime(".csv", "text/csv");
	// d
	this->buildMime(".doc", "application/msword");
	this->buildMime(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	// e
	this->buildMime(".eot", "application/vnd.ms-fontobject");
	this->buildMime(".epub", "application/epub+zip");
	// g
	this->buildMime(".gif", "image/gif");
	// h
	this->buildMime(".htm", "text/html");
	this->buildMime(".html", "text/html");
	// i
	this->buildMime(".ico", "image/x-icon");
	this->buildMime(".ics", "text/calendar");
	// j
	this->buildMime(".jar", "application/java-archive");
	this->buildMime(".jpeg", "image/jpeg");
	this->buildMime(".jpg", "image/jpeg");
	this->buildMime(".js", "application/javascript");
	this->buildMime(".json", "application/json");
	// m
	this->buildMime(".mid", "audio/midi");
	this->buildMime(".midi", "audio/midi");
	this->buildMime(".mpeg", "video/mpeg");
	this->buildMime(".mppkg", "application/vnd.apple.installer+xml");
	// o
	this->buildMime(".odp", "application/vnd.oasis.opendocument.presentation");
	this->buildMime(".ods", "application/vnd.oasis.opendocument.spreadsheet");
	this->buildMime(".odt", "application/vnd.oasis.opendocument.text");
	this->buildMime(".oga", "audio/ogg");
	this->buildMime(".ogv", "video/ogg");
	this->buildMime(".ogx", "application/ogg");
	this->buildMime(".otf", "font/otf");
	// p
	this->buildMime(".png", "image/png");
	this->buildMime(".pdf", "application/pdf");
	this->buildMime(".ppt", "application/vnd.ms-powerpoint");
	this->buildMime(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
	// r
	this->buildMime(".rar", "application/x-rar-compressed");
	this->buildMime(".rtf", "application/rtf");
	// s
	this->buildMime(".sh", "application/x-sh");
	this->buildMime(".svg", "image/svg+xml");
	this->buildMime(".swf", "application/x-shockwave-flash");
	// t
	this->buildMime(".tar", "application/x-tar");
	this->buildMime(".tif", "image/tiff");
	this->buildMime(".tiff", "image/tiff");
	this->buildMime(".ts", "application/typescript");
	this->buildMime(".ttf", "font/ttf");
	// v
	this->buildMime(".vsd", "application/vnd.visio");
	// w
	this->buildMime(".wav", "audio/x-wav");
	this->buildMime(".weba", "image/tiff");
	this->buildMime(".webm", "image/tiff");
	this->buildMime(".woff", "application/typescript");
	this->buildMime(".woff2", "font/ttf");
	// x
	this->buildMime(".xhtml", "application/xhtml+xml");
	this->buildMime(".xls", "application/vnd.ms-excel");
	this->buildMime(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	this->buildMime(".xml", "application/xml");
	this->buildMime(".xul", "application/vnd.mozilla.xul+xml");
	// z
	this->buildMime(".zip", "application/zip");
	this->buildMime(".7z", "application/x-7z-compressed");
	return ;
}
void				Response::buildMessages(int key, const std::string& mapped)
{
	std::pair<int, std::string>	elem(key, mapped);
	this->_messages.insert(elem);
	return ;
}
void				Response::setMessagesMap()
{
	// 1xx
	this->buildMessages(100, "Continue");
	this->buildMessages(101, "Switching Protocol");
	this->buildMessages(102, "Processing");
	this->buildMessages(103, "Early Hints");
	// 2xx
	this->buildMessages(200, "OK");
	this->buildMessages(201, "Created");
	this->buildMessages(202, "Accepted");
	this->buildMessages(203, "Non-Authoritative Information");
	this->buildMessages(204, "No Content");
	this->buildMessages(205, "Reset Content");
	this->buildMessages(206, "Partial Content");
	this->buildMessages(207, "Multi-Status");
	this->buildMessages(208, "Already Reported");
	this->buildMessages(226, "IM Used");
	// 3xx
	this->buildMessages(300, "Multiple Choice");
	this->buildMessages(301, "Moved Permanently");
	this->buildMessages(302, "Found");
	this->buildMessages(303, "See Other");
	this->buildMessages(304, "Not Modified");
	this->buildMessages(307, "Temporary Redirect");
	this->buildMessages(308, "Permanent Redirect");
	// 4xx
	this->buildMessages(400, "Bad Request");
	this->buildMessages(401, "Unauthorized");
	this->buildMessages(402, "Payment Required");
	this->buildMessages(403, "Forbidden");
	this->buildMessages(404, "Not Found");
	this->buildMessages(405, "Method Not Allowed");
	this->buildMessages(406, "Method Not Acceptable");
	this->buildMessages(407, "Proxy Authentification Required");
	this->buildMessages(408, "Request Timeout");
	this->buildMessages(409, "Conflict");
	this->buildMessages(410, "Gone");
	this->buildMessages(411, "Lenght Required");
	this->buildMessages(412, "Precondition Failed");
	this->buildMessages(413, "Payload Too Large");
	this->buildMessages(414, "URI Too Long");
	this->buildMessages(415, "Unsupported Media Type");
	this->buildMessages(416, "Requested Range Not Satisiable");
	this->buildMessages(417, "Excpectation Failed");
	this->buildMessages(418, "I'm a teapot");
	this->buildMessages(421, "Misdirected Request");
	this->buildMessages(422, "Unprocessable Entity");
	this->buildMessages(423, "Locked");
	this->buildMessages(424, "Failed Dependency");
	this->buildMessages(425, "TooEarly");
	this->buildMessages(426, "Upgrade Required");
	this->buildMessages(428, "Precondition Required");
	this->buildMessages(429, "Too Many Requests");
	this->buildMessages(431, "Request Header Fields Too Large");
	this->buildMessages(451, "Unavailable For Legal Reasons");
	// 5xx
	this->buildMessages(500, "Internal Server Error");
	this->buildMessages(501, "Not Implemented");
	this->buildMessages(502, "Bad Gateway");
	this->buildMessages(503, "Service Unavailable");
	this->buildMessages(504, "Gateway Timeout");
	this->buildMessages(505, "HTTP Version Not Supported");
	this->buildMessages(506, "Variant Also Negotiates");
	this->buildMessages(507, "Insufficient Storage");
	this->buildMessages(508, "Loop Detected");
	this->buildMessages(510, "Not Extended");
	this->buildMessages(511, "Network Authentication Required");
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
		fill_resp(itf->first + ":" + itf->second + "\r\n");
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

	fill_resp("Connection: keep-alive\r\n");
	fill_resp("Keep-Alive: timeout=3\r\n");

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
