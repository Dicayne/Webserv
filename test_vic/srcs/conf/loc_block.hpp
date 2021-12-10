/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_block.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:20 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/10 16:16:33 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOC_BLOCK_HPP
#define LOC_BLOCK_HPP

#include "../../../incs/webserv.hpp"

#include <vector>
#include <map>

class loc_block
{
private:
	/* data */
public:
	loc_block(/* args */);
	~loc_block();

	// PARSING
	void pars_loc(std::vector<std::string> block, std::string path);
};

#endif
