/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:33:59 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 13:56:55 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"

AResponse::AResponse(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& body)
{
    this->setProtocolVersion(protocol_version);
    this->setStatus(status);
    this->setStatusMessage(status_message);
    this->setStatusMessage(body);
    return ;
}

void	AResponse::setProtocolVersion(const std::string& pv){ this->protocol_version = pv; return ; }
void    AResponse::setStatus(const std::string& s){ this->status = s; return ; }
void    AResponse::setStatusMessage(const std::string& sm){ this->status_message = sm; return ; }
void    AResponse::setStatusMessage(const std::string& b){ this->body = b; return ; }

const std::string&	AResponse::getProtocolVersion() const{ return (this->protocol_version); }
const std::string&	AResponse::getStatus() const{ return (this->status); }
const std::string&	AResponse::getStatusMessage() const{ return (this->status_message); }
const std::string&  AResponse::getBody() const{ return (this->body); }

const std::string&   AResponse::buildResponse() const
{
    std::string             response;
    const char*             r = this->protocol_version.c_str();
    std::string::iterator   it = response.begin();
    
    response.insert(*it, r);
    response.insert(*it, "\n");


}