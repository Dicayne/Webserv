/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:27:09 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 17:22:16 by mabriand         ###   ########.fr       */
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
void	AResponse::setProtocolVersion(const std::string& pv){ this->protocol_version = pv; return ; }
void	AResponse::setStatus(const std::string& s){ this->status = s; return ; }
void	AResponse::setStatusMessage(const std::string& sm){ this->status_message = sm; return ; }
void	AResponse::setBody(const std::string& b){ this->body = b; return ; }


const std::string&	AResponse::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&	AResponse::getStatus() const{ return (this->status); }
const std::string&	AResponse::getStatusMessage() const{ return (this->status_message); }
const std::string&  AResponse::getBody() const{ return (this->body); }




const std::string&  AResponse::getResponse() const{ return (this->response); }///////////////

void	AResponse::buildResponse()
{
	std::string				response(this->response);
	const char*				r = this->protocol_version.c_str();
	const char*				s = this->status.c_str();
	const char*				t = this->status_message.c_str();
	const char*				u = this->body.c_str();
	std::cout << YELLOW << "[" << u << "]" << NC << std::endl;
	
	std::string::iterator	it = response.begin();
	size_t					i = 0;

	response.insert(i, r);
	while (it != response.end())
	{
		it++;
		i++;
	}
	response.insert(i, "\n");
	it++;
	i++;

	response.insert(i, s);
	while (it != response.end())
	{
		it++;
		i++;
	}
	response.insert(i, "\n");
	it++;
	i++;

	response.insert(i, t);
	while (it != response.end())
	{
		it++;
		i++;
	}
	response.insert(i, "\n");
	it++;
	i++;

	response.insert(i, u);
	// while (it != response.end())
	// {
	// 	it++;
	// 	i++;
	// }
	// response.insert(i, "\n");
	// it++;
	// i++;



	this->response = response;
	
	return ;
}

void*  AResponse::respond() const
{
	return ((void *)(this->response.c_str()));
}

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