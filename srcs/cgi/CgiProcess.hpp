/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:34:41 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/29 21:22:26 by mabriand         ###   ########.fr       */
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
		std::vector< char >			_newBody;

	public:
		CgiProcess(Request *current_request, Server *current_server);
		~CgiProcess();

		bool	isCgiNeeded();
		int		findPosVar(std::string var);
		

		void	set_myEnv();
		void	set_envVars();
		

	
		const std::string	get_Var(std::string var);
		void				clearEnv();

		
		// int		createCommunication();
		// void		launchCgiProcess();
		int					exeCgiProgram();
		
		

};

#endif

