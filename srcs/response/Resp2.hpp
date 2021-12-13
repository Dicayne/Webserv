/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 13:07:32 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/13 14:39:07 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESP2_HPP
# define RESP2_HPP

#include "../incs/webserv.hpp"
#include "./AResponse.hpp"
class   Resp2 : public AResponse
{
    private:
        Resp2();

    public:
		Resp2(const std::string& protocol_version, const std::string& status, const std::string& url);
		virtual ~Resp2();
};

#endif