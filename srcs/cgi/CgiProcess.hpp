/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:34:41 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/15 03:06:50 by vmoreau          ###   ########.fr       */
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

		Request								*_request;
		Server								*_server;
		std::vector<std::string>			_envVars;
		std::map<std::string, std::string> 	_env_map;
		char								**_myEnv;
		std::vector< char >					_cgi_output;
		std::vector< char >					_cgi_head;

	public:
		CgiProcess(Request *current_request, Server *current_server);
		~CgiProcess();

		void		init();
		bool		isCgiNeeded();
		int			findPosVar(std::string var);
		std::string	get_cwd(void);

		void		set_myEnv();
		void		set_envVars();
		void		set_cgiOutput(std::string	body);

		const std::string	get_Var(std::string var);
		std::vector< char >	get_cgiOutput();
		// std::vector< char >	get_cgiHead();
		void				clearEnv();


		// int		createCommunication();
		// void		launchCgiProcess();
		int					exeCgiProgram();

		class CGI_ERR_PROCESS : public std::exception
		{
			private:
				std::string _msg;
			public:
				CGI_ERR_PROCESS(std::string msg) : _msg(msg){}
				virtual ~CGI_ERR_PROCESS() throw() {}
				virtual const char *what() const throw() {return (this->_msg.c_str());}
		};
};

#endif

