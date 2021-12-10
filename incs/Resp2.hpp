/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 13:07:32 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/10 22:37:37 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESP2_HPP
# define RESP2_HPP

#include "webserv.hpp"
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