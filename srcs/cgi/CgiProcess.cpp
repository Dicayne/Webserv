/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:40:07 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/28 13:44:32 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiProcess.hpp"

CgiProcess::CgiProcess(Request *current_request, Server *current_server) : _myEnv(NULL)
{
	this->_request = current_request;
	this->_server = current_server;
	this->set_envVars();
	this->set_myEnv();
	int i = 0;
	
	while (this->_myEnv[i] != NULL)
	{
		std::cout << PURPLE << this->_myEnv[i] << std::endl;
		++i;
	}
	
	return ;
}

CgiProcess::~CgiProcess()
{
	// this->clearEnv();
	return ;
}

bool     			CgiProcess::isCgiNeeded()
{
	std::string	temp = this->_request->getUrl();
	size_t		size = temp.size();
	size_t		idx = temp.find_last_of(".", size);
	std::string ext(temp.substr(idx, size - idx));
	// std::cout << GREEN << "temp is: " << temp << "\n" << NC;
	// std::cout << GREEN << "ext is: " << ext << "\n" << NC;
	if (idx == std::string::npos)
		return (false);
	else if (ext.compare(".php") != 0)
		return (false);
	// std::cout << GREEN << "=====> CGI NEEDED HERE : if not, it wont load the PHP file!\n" << NC;
	return (true);
}

void				CgiProcess::set_envVars()
{
	this->_envVars.push_back("CONTENT_LENGTH");
	this->_envVars.push_back("CONTENT_TYPE");
	this->_envVars.push_back("GATEWAY_INTERFACE");
	this->_envVars.push_back("PATH_INFO");
	this->_envVars.push_back("PATH_TRANSLATED");
	this->_envVars.push_back("QUERY_STRING");
	this->_envVars.push_back("REDIRECT_STATUS");
	this->_envVars.push_back("REQUEST_METHOD");
	this->_envVars.push_back("SCRIPT_FILENAME");
	this->_envVars.push_back("SERVER_PROTOCOL");
	this->_envVars.push_back("SERVER_PORT");
	// this->_envVars.push_back("");

	return ;
}

void				CgiProcess::set_myEnv()
{
	size_t		size = this->_envVars.size();
	// std::cout << "size of this->_envVars() = " << size << std::endl;
	// std::string	end = "";
	this->_myEnv = (char **)malloc(sizeof(char *) * (size + 1));
	if (!this->_myEnv)
		return ; // doit renvoyer une erreur, les autres ont mis des throw
	// this->_myEnv[size + 1] = NULL;
	// for (int i = 0; (size_t)i < size + 1; ++i)
	for (int i = 0; (size_t)i < size; ++i)
	{
		std::string tmp = this->_envVars[i] + "=" + this->get_Var(this->_envVars[i]);
		this->_myEnv[i] = strdup(tmp.c_str());
		// std::cout << "-------------------------------------------------> i is: " << i << std::endl;
		// if (!this->_myEnv[i])
		// 	return ; //
	}
	// std::string tmp = "";
	// std::cout << "-------------------------------------------------> size is: " << size << std::endl;
	this->_myEnv[size] = NULL;
	// this->_myEnv[size] = strdup(tmp.c_str());
}

int					CgiProcess::findPosVar(std::string var)
{
	size_t	size = this->_envVars.size();
	for (int i = 0;(size_t)i < size; ++i)
	{
		if (var.compare(this->_envVars[i].c_str()) == 0)
			return (i);
	}
	return (-1);
}

const std::string	CgiProcess::get_Var(std::string var)
{
	int	pos = this->findPosVar(var);
	switch (pos)
	{
		case 0: 
			if (this->_request->getMethod() == "GET")
				return (std::to_string(this->_request->get_queryString().size())); //query string pareil ?
			return (std::to_string(this->_request->getBody().size())); //query string pareil ?
		case 1: return (""); //Le type MIME du corps de la requête, ou null si le type n'est pas connu.
		case 2: return ("CGI/1.1"); // vu une technique sympa ou ça et case9 sont notés en #define : mieux
		case 3: return (this->_request->get_baseUrl());
		case 4: return (this->_request->getUrl());
		case 5: return (this->_request->get_queryString());
		case 6: return (std::to_string(this->_request->returnStatusCode())); //pas sure que ce soit le bon si ya redirection?
		case 7: return (this->_request->getMethod());
		case 8: return (this->_request->getUrl());
		case 9: return ("HTTP/1.1");
		case 10: return (std::to_string(this->_request->getBlock()->get_port()));
		default: return ("");
	}
}

void				CgiProcess::clearEnv()
{
	if (this->_myEnv != NULL)
	{
		for (int i = 0; this->_myEnv[i]; ++i)
			free(this->_myEnv[i]);
		free(this->_myEnv);
		this->_myEnv = NULL;
	}
	return ;
}

// int	CgiProcess::createCommunication()
// {
// 	if (this->_request->getMethod().compare("POST") == 0)
// 	{
// 		//mettre data dans QUERY_STRING
// 	}
// 	else if (this->_request->getMethod().compare("GET") == 0)
// 	{
// 		//mettre data dans STDIN
// 	}
// 	else
// 		return (-1);
// }

int					CgiProcess::exeCgiProgram()
{
	int	fd_backUp[2];
	fd_backUp[0] = dup(STDIN_FILENO);	//crée une copie de STDIN_FILENO, renvoie le fd de la copie
	fd_backUp[1] = dup(STDOUT_FILENO);	//crée une copie de STDOUT_FILENO, renvoie le fd de la copie
	if (fd_backUp[0] == -1 || fd_backUp[1] == -1)	//Si ça échoue
		return (1);
	
	FILE	*fileIn = tmpfile(); 	// The tmpfile() function opens a unique temporary file in binary
    FILE	*fileOut = tmpfile();	//	read/write (w+b) mode.  The file will be automatically deleted when it is closed or the program terminates.
	if (fileIn == NULL || fileOut == NULL)
		return (1);					

	long	fd_in = fileno(fileIn);
	long	fd_out = fileno(fileOut);
	if (fd_in == -1 || fd_out == -1)
		return (1);

	write(fd_in, this->_request->getBody().c_str(),  this->_request->getBody().size());
	lseek(fd_in, 0, SEEK_SET);

	
	std::string	cgi_path = "./www/bin/";
	std::string cgi_file = "./html/action.php";
		
	char * argv[3] = {
		const_cast<char*>(cgi_path.c_str()),
		const_cast<char*>(cgi_file.c_str()),
		(char *)0
	};
	std::cout << GREEN << "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu" << argv[0] << NC << std::endl;
	std::cout << GREEN << "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu" << argv[1] << NC << std::endl;
	pid_t	pid = fork(); //je fork le processus parent cad le server
	if (pid < 0)
	{
		std::cout << RED << "PID < 0" << NC << std::endl;
		return (-1);//erreur 500 je crois
	}
	else if (pid == 0) // processus fils
	{
		std::cout << GREEN << "FILS" << NC << std::endl;	
		if (dup2(fd_in, 0) < 0)
        {
			std::cout << YELLOW << "dup2(fd_in, 0) < 0" << NC << std::endl;	
			exit(EXIT_FAILURE);
			return (1);
		}  //erreur;
    	if (dup2(fd_out, 1) < 0)
		 {
			std::cout << YELLOW << "dup2(fd_out, 1) < 0" << NC << std::endl;	
			exit(EXIT_FAILURE);
			return (1);
		}
		// char * const * nll = NULL;
        // if (execve("./www/bin/", /*peut-eêtre NULL ? Ou bien le fichier .php à exécuter*/, this->_myEnv) < 0) //
		std::cout << YELLOW << "testing execve" << NC << std::endl;
		
		execve(argv[0], &argv[0], this->_myEnv);
		std::cerr << RED << "Execve crashed." << NC << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		// if (execve("../../www/bin/", nll, this->_myEnv) < 0)
        // {
		// 	std::cout << YELLOW << "execve FAILED" << NC << std::endl;
		// 	exit(EXIT_FAILURE);
		// 	return (-1);
		// }
		// std::cout << YELLOW << "execve SUCCEEDED" << NC << std::endl;
		// exit(EXIT_SUCCESS);
		// return (0);
	}
	else // processus père
	{
		std::cout << GREEN << "PARENT" << NC << std::endl;
		// exit(EXIT_SUCCESS);
		// return (0);

		
		char	buffer[4096] = {0};

		waitpid(-1, NULL, 0);
		lseek(fd_out, 0, SEEK_SET);
	
		std::string	newBody;
		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 4096);
			ret = read(fd_out, buffer,  4096 - 1);
			newBody += buffer; // new body devient le body dans Response ??
		}
		std::cout << BLUE << newBody << NC << std::endl;
	}
	if (dup2(fd_backUp[0], 0) < 0)
		return (1); //erreur;
    if (dup2(fd_backUp[1], 1) < 0)
		return (1);
	
	fclose(fileIn);
	fclose(fileOut);
	close(fd_in);
	close(fd_out);
	close(fd_backUp[0]);
	close(fd_backUp[1]);
 	// return (0);
	// for (size_t i = 0; this->_myEnv[i]; i++)
	// 	delete[] this->_myEnv[i];
	// delete[] this->_myEnv;

	if (!pid)
		exit(0);
	return (0);
}
