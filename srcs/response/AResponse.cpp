/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:27:09 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/13 18:48:17 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./AResponse.hpp"

AResponse::AResponse(){}
AResponse::AResponse(const std::string& protocol_version, const std::string& status, const std::string& url)
{
	// Independant from the parameters
	this->setMimeMap();
	this->setMessagesMap();


	// Dependant from the parameters
	this->setProtocolVersion(protocol_version);
	this->setStatus(status);
	this->setStatusMessage(status);
	this->setDate();
	// this->setServer();
	this->setBody(url);
	this->setContentType(url);
	this->setContentLenght();

	this->buildResponse();
	return ;

}
AResponse::~AResponse(){}

void	AResponse::setProtocolVersion(const std::string& protocol_version)
{
	this->protocol_version = protocol_version;
	std::pair<std::string, std::string> elem("Protocol-Version", this->protocol_version);
	this->stock.insert(elem);
	return ;
}
void	AResponse::setStatus(const std::string& status)
{
	this->status = status;
	std::pair<std::string, std::string> elem("Status", this->status);
	this->stock.insert(elem);
	return ;
}
void	AResponse::setStatusMessage(const std::string& status)
{
	(void)status;
	int key = 200;
	std::map<int, std::string>::iterator	itf = (this->messages).find(key);
	this->status_message = itf->second;
	std::pair<std::string, std::string> elem("Status-Message", this->status_message);
	this->stock.insert(elem);
	return ;



	// this->status_message = status_message;
	// std::pair<std::string, std::string> elem("Status-Message", this->status_message);
	// this->stock.insert(elem);
	return ;
}
void	AResponse::setDate()
{
    std::time_t result = std::time(nullptr);
	std::string date(std::asctime(std::localtime(&result)));
	date.pop_back();

	this->date = date;
	std::pair<std::string, std::string> elem("Date", this->date);
	this->stock.insert(elem);
	return ;
}
///////////////////////////////////
void	AResponse::setContentType(const std::string& url)
{
	size_t pos = url.find_last_of(".", url.size());
	if (pos == std::string::npos)
		return ;

	std::string key(url.substr(pos, url.size() - pos));
	std::map<std::string, std::string>::iterator	itf = (this->mime).find(key);
	this->content_type = itf->second;
	std::pair<std::string, std::string> elem("Content-Type", this->content_type);
	this->stock.insert(elem);
	return ;
}
void	AResponse::setContentLenght()
{
	std::stringstream content_lenght;
	content_lenght << this->body.size();

	this->content_lenght = content_lenght.str();
	std::pair<std::string, std::string> elem("Content-Lenght", this->content_lenght);
	this->stock.insert(elem);
	return ;
}
void	AResponse::setBody(const std::string& url)
{
	std::ifstream ms;
	std::string	temp;
	std::string body;

	ms.open(url);
	while(std::getline(ms, temp))
	{
		body += temp;
		body += '\n';
	}
	ms.close();

	body.pop_back();
	this->body = body;

	std::pair<std::string, std::string> elem("Body", this->body);
	this->stock.insert(elem);
	return ;
}

const std::string&	AResponse::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&	AResponse::getStatus() const{ return (this->status); }
const std::string&	AResponse::getStatusMessage() const{ return (this->status_message); }
const std::string&	AResponse::getDate() const{ return (this->date); }
//
const std::string&  AResponse::getBody() const{ return (this->body); }
const std::string&	AResponse::getContentType() const{ return (this->content_type); }
const std::string&	AResponse::getContentLenght() const{ return (this->content_lenght); }
//
const std::string&  AResponse::getResponse() const{ return (this->response); }///////////////

void	AResponse::buildMime(const std::string& key, const std::string& mapped)
{
	std::pair<std::string, std::string>	elem(key, mapped);
	this->mime.insert(elem);
	return ;
}

void	AResponse::setMimeMap()
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
void	AResponse::buildMessages(int key, const std::string& mapped)
{
	std::pair<int, std::string>	elem(key, mapped);
	this->messages.insert(elem);
	return ;
}
void	AResponse::setMessagesMap()
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
	this->buildMessages(505, "HTTP VErsion Not Supported");
	this->buildMessages(506, "Variant Also Negotiates");
	this->buildMessages(507, "Insufficient Storage");
	this->buildMessages(508, "Loop Detected");
	this->buildMessages(510, "Not Extended");
	this->buildMessages(511, "Network Authentication Required");
	return ;
}

// void	AResponse::buildLineResp(const std::string str1, const std::string sep1, const std::string str2, const std::string sep2, int *i)
// {
// 	if (!str1.empty())
// 		*i = (this->response).insert(*i, str1).size();
// 	if (!sep1.empty())
// 		*i = (this->response).insert(*i, sep1).size();
// 	if (!str2.empty())
// 		*i = (this->response).insert(*i, str2).size();
// 	if (!sep2.empty())
// 		*i = (this->response).insert(*i, sep2).size();
// 	return ;

// }
void	AResponse::buildPartResp(const std::string& key, int *i)
{
	std::map<std::string, std::string>::iterator	itf = (this->stock).find(key);

	(void) i;
	if (key.compare("Protocol-Version") == 0 || key.compare("Status") == 0)
		this->response += itf->second;
	else if (key.compare("Status-Message") == 0)
		this->response += itf->second + "\n";									// Moyen Plus simple de fill la reponse, probleme si tu envoie le body d'une image avec c_str()
	else if (key.compare("Body") == 0)
		this->response += "\n" + itf->second;
	else
		this->response += itf->first + ":" + itf->second + "\n";

	// if (key.compare("Protocol-Version") == 0 || key.compare("Status") == 0)
	// 	return (this->buildLineResp(itf->second, " ", "", "", i));
	// else if (key.compare("Status-Message") == 0)
	// 	return (this->buildLineResp(itf->second, "\n", "", "", i));
	// else if (key.compare("Body") == 0)
	// 	return (this->buildLineResp("\n", "", itf->second, "", i));
	// return (this->buildLineResp(itf->first, ": ", itf->second, "\n", i));
}
void	AResponse::buildResponse()
{
	int i = 0;

	this->buildPartResp("Protocol-Version", &i);
	this->buildPartResp("Status", &i);
	this->buildPartResp("Status-Message", &i);
	this->buildPartResp("Date", &i);

	this->buildPartResp("Content-Type", &i);
	this->buildPartResp("Content-Lenght", &i);

	this->buildPartResp("Body", &i);

	return ;
}

void*  AResponse::respond() const{ return ((void *)(this->response.c_str())); }

std::ostream&	operator<<(std::ostream& os, const AResponse& r)
{
	os << "[" << r.getProtocolVersion() << "]" << std::endl;
	os << "[" << r.getStatus() << "]" << std::endl;
	os << "[" << r.getStatusMessage() << "]" << std::endl;
	os << "[" << r.getContentType() << "]" << std::endl;
	os << "[" << r.getContentLenght() << "]" << std::endl;
	os << "[" << r.getBody() << "]" << std::endl;
	os << YELLOW << "===>" << r.getResponse() << "<===" << NC << std::endl;
	return (os);
}