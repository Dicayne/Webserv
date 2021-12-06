/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 13:20:30 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/06 16:12:54 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/color.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void clear_buf(char buff[1024])
{
	int i = 0;

	while (i < 1024)
	{
		buff[i] = '\0';
		i++;
	}
}

int main()
{
	std::string msg , recep;
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Socket creation error\n";
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Invalid adress / address not supported\n";
		exit(EXIT_FAILURE);
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << RED << "Error=> " << NC << "Connection to socket failed\n";
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		std::cout << "Send: ";
		std::getline(std::cin, msg);
		if (send(sock, msg.c_str(), msg.size() , 0) < 0)
		{
			std::cerr << RED << "Error=> " << NC << "Message not send\n";
			exit(EXIT_FAILURE);
		}
		std::cout << GREEN << "Message Send\n" << NC;
		recv(sock, buffer, 1024, 0);
		recep = buffer;
		std::cout << BLUE << "Server:		" << NC << recep << "\n\n";
		clear_buf(buffer);
	}
	return (0);
}