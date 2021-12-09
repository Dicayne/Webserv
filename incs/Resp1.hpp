/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resp1.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:55:42 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/09 13:15:11 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESP1_HPP
# define RESP1_HPP

#include "webserv.hpp"

class   Resp1 : public AResponse
{
    private:
        Resp1();

    public:
		Resp1(const std::string& y);
		~Resp1();
}

#endif