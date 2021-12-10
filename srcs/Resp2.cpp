/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:19:13 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/10 22:38:13 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Resp2.hpp"

Resp2::Resp2(const std::string& protocol_version, const std::string& status, const std::string& url):
    AResponse(protocol_version, status, url){ std::cout << *this << std::endl; return ; }
Resp2::~Resp2(){}