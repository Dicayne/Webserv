/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_block.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:18 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/10 16:19:09 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loc_block.hpp"

loc_block::loc_block(/* args */)
{
}

loc_block::~loc_block()
{
}

// ********************* PARSING ********************* //

void loc_block::pars_loc(std::vector<std::string> block, std::string path)
{
	std::cout << path << '\n';
	for (size_t i = 0; i < block.size(); i++)
	{
		std::cout << block[i] << '\n';
	}

}
