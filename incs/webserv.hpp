/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:53:05 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/13 14:55:39 by vmoreau          ###   ########.fr       */
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
#include <fcntl.h>			// fcntl, SEUL UTILISATION POSSIBLE = fcntl(fd, F_SETFL, O_NONBLOCK);

// #include <string_view>
#include <map>
#include <bitset>
#include <sstream>
#include <ctime>

// #include "./Request.hpp"
// #include "./AResponse.hpp"
// #include "./Resp1.hpp"
// #include "./Resp2.hpp"
// #include "./Resp3.hpp"
// #include "./Resp4.hpp"
// #include "./Resp5.hpp"

// #include <sys/types.h>
// #include <netinet/in.h>


// *UTILS* //

#include "color.hpp"


#define CONF_DEFAULT_PATH "./srcs/conf.d/used/default.conf"

#endif