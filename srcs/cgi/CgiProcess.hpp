/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:34:41 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/29 23:11:38 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_PROCESS_HPP
# define CGI_PROCESS_HPP

#include "../incs/webserv.hpp"

#include "../request/Request.hpp"
#include "../server/Server.hpp"


class   CgiProcess
{
	private:
		CgiProcess();
		CgiProcess(CgiProcess& toCopy);
		CgiProcess&	operator=(CgiProcess& toAssign);
		
		Request						*_request;
		Server						*_server;
		std::vector<std::string>	_envVars;
		char						**_myEnv;
		std::vector< char >			_cgi_output;
		std::vector< char >			_cgi_head;

	public:
		CgiProcess(Request *current_request, Server *current_server);
		~CgiProcess();

		bool	isCgiNeeded();
		int		findPosVar(std::string var);
		

		void	set_myEnv();
		void	set_envVars();
		void	set_cgiOutput(std::string	body);

	
		const std::string	get_Var(std::string var);
		std::vector< char >	get_cgiOutput();
		// std::vector< char >	get_cgiHead();
		void				clearEnv();

		
		// int		createCommunication();
		// void		launchCgiProcess();
		int					exeCgiProgram();
		
		

};

#endif

