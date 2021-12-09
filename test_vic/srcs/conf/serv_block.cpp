/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_block.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:06 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/09 18:34:03 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serv_block.hpp"

serv_block::serv_block()
{
	// std::cout << "Serv_Block construct\n";
}

serv_block::~serv_block()
{
}

// ********************* PARSING ********************* //

void serv_block::pars_serv(std::vector<std::string> block)
{
	this->_block = block;
	for (size_t i = 0; i < this->_block.size(); i++)
		std::cout << YELLOW << this->_block[i] << NC << '\n';
	std::cout << "\n\n";

}
