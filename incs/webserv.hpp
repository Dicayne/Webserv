/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:53:05 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/06 13:48:11 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <string>

// *INCLUDE FOR FUNCTION AUTORIZED* //

#include <cstdlib>			// malloc / free /
#include <unistd.h>			// write /
#include <arpa/inet.h>		// htons / htonl / ntohs / ntohl / inet_adrr
#include <sys/select.h> 	// select /
#include <poll.h>			// poll /
#include <sys/event.h>		// kqueu / kevent /
#include <sys/socket.h>		// socket / accept / listen / send / recv / bind / setsockopt / getsockname /
#include <fcntl.h>			// fcntl, SEUL UTILISATION POSSIBLE = fcntl(fd, F_SETFL,O_NONBLOCK);

// *UTILS* //

#include "color.hpp"



#endif