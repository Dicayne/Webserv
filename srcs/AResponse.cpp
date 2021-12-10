/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:27:09 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/10 20:01:34 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/AResponse.hpp"

AResponse::AResponse(){}
AResponse::AResponse(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& content_type,/* const std::string& content_lenght,*/ const std::string& body)
{
	this->setProtocolVersion(protocol_version);
	this->setStatus(status);
	this->setStatusMessage(status_message);
	this->setDate();
	// this->setServer();
	this->setBody(body);
	this->setContentType(content_type);
	this->setContentLenght(/*content_lenght*/);
	
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
void	AResponse::setStatusMessage(const std::string& status_message)
{
	this->status_message = status_message;
	std::pair<std::string, std::string> elem("Status-Message", this->status_message);		
	this->stock.insert(elem);
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
void	AResponse::setContentType(const std::string& content_type)
{
	this->content_type = content_type;
	std::pair<std::string, std::string> elem("Content-Type", this->content_type);		
	this->stock.insert(elem);
	return ;
}
void	AResponse::setContentLenght(/*const std::string& content_lenght*/)
{
	std::stringstream content_lenght;
	content_lenght << this->body.size();

	this->content_lenght = content_lenght.str();
	std::pair<std::string, std::string> elem("Content-Lenght", this->content_lenght);		
	this->stock.insert(elem);
	return ;
}
void	AResponse::setBody(const std::string& b)
{ 
	this->body = b;
	std::pair<std::string, std::string> elem("Body", this->body);		
	this->stock.insert(elem);
	// std::cout << "|" << this->body << "|" << std::endl;
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
	// f
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
	// k
	// l
	// m
	this->buildMime(".mid", "audio/midi");
	this->buildMime(".midi", "audio/midi");
	this->buildMime(".mpeg", "video/mpeg");
	this->buildMime(".mppkg", "application/vnd.apple.installer+xml");
	// n
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

	

}


void	AResponse::buildLineResp(const char *str1, const char *sep1, const char *str2, const char *sep2, int *i)
{
	if (str1)
		*i = (this->response).insert(*i, str1).size();
	if (sep1)
		*i = (this->response).insert(*i, sep1).size();
	if (str2)	
		*i = (this->response).insert(*i, str2).size();
	if (sep2)
		*i = (this->response).insert(*i, sep2).size();
	return ;

}
void	AResponse::buildPartResp(const std::string& key, int *i)
{
	std::map<std::string, std::string>::iterator	itf = (this->stock).find(key);
	
	if (key.compare("Protocol-Version") == 0 || key.compare("Status") == 0)
		return (this->buildLineResp(itf->second.c_str(), " ", NULL, NULL, i));
	else if (key.compare("Status-Message") == 0)
		return (this->buildLineResp(itf->second.c_str(), "\n", NULL, NULL, i));
	else if (key.compare("Body") == 0)
		return (this->buildLineResp("\n", NULL, itf->second.c_str(), NULL, i));
	// else if (key.compare("Date") == 0)
	// 	return (this->buildLineResp(itf->first.c_str(), ": ", itf->second.c_str(), "", i));
	return (this->buildLineResp(itf->first.c_str(), ": ", itf->second.c_str(), "\n", i));
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
	os << "Message sent to the client as response:\n\n" << std::endl;
	os << r.getResponse() << std::endl;
	return (os);
}