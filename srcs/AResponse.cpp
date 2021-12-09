/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:27:09 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 18:55:29 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/AResponse.hpp"

AResponse::AResponse(){}
AResponse::AResponse(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& body)
{
	this->setProtocolVersion(protocol_version);
	this->setStatus(status);
	this->setStatusMessage(status_message);
	this->setBody(body);
	
	this->buildResponse();
	return ;

}
AResponse::~AResponse(){}

void	AResponse::setProtocolVersion(const std::string& pv)
{
	this->protocol_version = pv;
	std::pair<std::string, std::string> elem("protocol_version", this->protocol_version);		
	this->stock.insert(elem);
	return ; 
}
void	AResponse::setStatus(const std::string& s)
{
	this->status = s;
	std::pair<std::string, std::string> elem("status", this->status);		
	this->stock.insert(elem);
	return ;
}
void	AResponse::setStatusMessage(const std::string& sm)
{
	this->status_message = sm;
	std::pair<std::string, std::string> elem("status_message", this->status_message);		
	this->stock.insert(elem);
	return ;
}
void	AResponse::setBody(const std::string& b)
{ 
	this->body = b;
	std::pair<std::string, std::string> elem("body", this->body);		
	this->stock.insert(elem);
	return ;
}

const std::string&	AResponse::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&	AResponse::getStatus() const{ return (this->status); }
const std::string&	AResponse::getStatusMessage() const{ return (this->status_message); }
const std::string&  AResponse::getBody() const{ return (this->body); }

const std::string&  AResponse::getResponse() const{ return (this->response); }///////////////

void	AResponse::buildPartResp(const std::string& key, int *i)
{
	std::cout << BLUE << *this << NC;
	std::map<std::string, std::string>::iterator	itf = (this->stock).find(key);
	
	(this->response).insert(*i, (const char *)itf->second.c_str());
	*i = (this->response).size();
	
	if (key.compare("body") != 0)
	{
		(this->response).insert(*i, "\n");
		*i += 1;
	}
	return ;
}
void	AResponse::buildResponse()
{
	int i = 0;

	this->buildPartResp("protocol_version", &i);
	this->buildPartResp("status", &i);
	this->buildPartResp("status_message", &i);
	this->buildPartResp("body", &i);
	
	return ;
}

void*  AResponse::respond() const{ return ((void *)(this->response.c_str())); }

std::ostream&	operator<<(std::ostream& os, const AResponse& r)
{
	os << "[" << r.getProtocolVersion() << "]" << std::endl;
	os << "[" << r.getStatus() << "]" << std::endl;
	os << "[" << r.getStatusMessage() << "]" << std::endl;
	os << "[" << r.getBody() << "]" << std::endl;
	os << "=====================" << std::endl;
	os << "[" << r.getResponse() << "]" << std::endl;
	return (os);
}