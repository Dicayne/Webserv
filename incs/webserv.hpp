/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:53:05 by vmoreau           #+#    #+#             */
/*   Updated: 2022/01/04 15:31:36 by vmoreau          ###   ########.fr       */
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
#include <sys/socket.h>		// socket / accept / listen / send / recv / bind / setsockopt / getsockname /
#include <netinet/in.h>
#include <arpa/inet.h>		// htons / htonl / ntohs / ntohl / inet_adrr
#include <sys/select.h> 	// select /
#include <poll.h>			// poll /
#include <sys/event.h>		// kqueu / kevent /
#include <fcntl.h>			// fcntl, SEUL UTILISATION POSSIBLE = fcntl(fd, F_SETFL, O_NONBLOCK);

// #include <string_view>
#include <map>
#include <bitset>
#include <sstream>
#include <ctime>

// #include <sys/types.h>
// #include <netinet/in.h>


// *UTILS* //

#include "color.hpp"

#define CONF_DEFAULT_PATH "./srcs/conf.d/used/default.conf"
#define DEFAULT_ERR_400 "./srcs/html/error/400.html"
#define DEFAULT_ERR_404 "./srcs/html/error/404.html"
#define DEFAULT_ERR_40x "./srcs/html/error/40x.html"
#define DEFAULT_ERR_500 "./srcs/html/error/500.html"
#define DEFAULT_ERR_50x "./srcs/html/error/50x.html"

#endif
