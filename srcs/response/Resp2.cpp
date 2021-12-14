/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:19:13 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/14 12:03:24 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Resp2.hpp"

Resp2::Resp2(const std::string& protocol_version, const std::string& status, const std::string& url): AResponse(protocol_version, status, url)
{ return ; }


Resp2::~Resp2(){}