/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp2.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 13:07:32 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 13:37:15 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESP2_HPP
# define RESP1_HPP

#include "webserv.hpp"

class   Resp2 : public AResponse
{
    private:
        Resp2();

    public:
		Resp2(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& body);
		~Resp2();
}

#endif