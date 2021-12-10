/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:19:13 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/10 16:17:35 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Resp2.hpp"

Resp2::Resp2(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& content_type,/* const std::string& content_lenght,*/ const std::string& body):
    AResponse(protocol_version, status, status_message, content_type,/* content_lenght,*/ body){}
Resp2::~Resp2(){}