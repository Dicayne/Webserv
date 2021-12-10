/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   AResponse.hpp									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mabriand <mabriand@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2021/12/08 15:54:04 by mabriand		  #+#	#+#			 */
/*   Updated: 2021/12/09 16:38:28 by mabriand		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef ARESPONSE_HPP
# define ARESPONSE_HPP

#include "webserv.hpp"

class   AResponse
{
	private:
		AResponse(AResponse& toCopy);
		AResponse&  operator=(AResponse& toAssign);

		std::string							response;
		std::map<std::string, std::string>	stock;
		std::map<std::string, std::string>	mime;

		/*
			Attributes corresponding to the fields of the HTTP request:
		*/
		std::string	protocol_version;
		std::string	status;
		std::string	status_message;
		std::string date;
		// std::string	server;
		std::string	body;
		std::string content_type;
		std::string content_lenght; // Content-Lenght : ne doit pas dépasser le client body size max
		

	public:
		AResponse();
		AResponse(const std::string& protocol_version, const std::string& status, const std::string& status_message, const std::string& url);
		virtual ~AResponse();

		/*
			All setters (one for each attribute corresponding to a field of the HTTP request):
		*/
		void	setProtocolVersion(const std::string& pv);
		void	setStatus(const std::string& s);
		void	setStatusMessage(const std::string& sm);
		void	setDate();
		// void	setServer();
		void	setContentType(const std::string& content_type);
		void	setContentLenght();
		void	setBody(const std::string& b);


		void	buildMime(const std::string& key, const std::string& mapped);
		void	setMimeMap();
		
		void	buildLineResp(const char *str1, const char *sep1, const char *str2, const char *sep2, int *i);
		void	buildPartResp(const std::string& key, int *i);
		void	buildResponse();
		void*	respond() const;




		/*
			All getters (one for each attribute corresponding to a field of the HTTP request):
		*/
		const std::string&	getProtocolVersion() const;
		const std::string&	getStatus() const;
		const std::string&	getStatusMessage() const;
		const std::string&  getDate() const;
		// const std::string&  getServer() const;
		const std::string&  getContentType() const;
		const std::string&	getContentLenght() const;
		const std::string&	getBody() const;

		const std::string&	getResponse() const;


		

};

std::ostream&	operator<<(std::ostream& os, const AResponse& r);

#endif