/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:19:13 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/13 16:20:09 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Resp2.hpp"

Resp2::Resp2(const std::string& protocol_version, const std::string& status, const std::string& url):
    AResponse(protocol_version, status, url)
	{
		// std::cout << GREEN << *this << NC << '\n';
		/*std::cout << *this << std::endl;*/
		return ;
	}


Resp2::~Resp2(){}