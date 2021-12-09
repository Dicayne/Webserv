/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:27:09 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 15:36:30 by mabriand         ###   ########.fr       */
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
	return ;
}
AResponse::~AResponse(){}
void	AResponse::setProtocolVersion(const std::string& pv){ this->protocol_version = pv; return ; }
void	AResponse::setStatus(const std::string& s){ this->status = s; return ; }
void	AResponse::setStatusMessage(const std::string& sm){ this->status_message = sm; return ; }
void	AResponse::setBody(const std::string& b){ this->body = b; return ; }

const std::string&	AResponse::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&	AResponse::getStatus() const{ return (this->status); }
const std::string&	AResponse::getStatusMessage() const{ return (this->status_message); }
const std::string&  AResponse::getBody() const{ return (this->body); }

const void*			AResponse::buildResponse() const
{
	std::string				response;
	const char*				r = this->protocol_version.c_str();
	
	std::string::iterator	it = response.begin();
	response.insert(*it, r);
	it += this->protocol_version.size();
	response.insert(*it, "\n");
	return (response.c_str());
}