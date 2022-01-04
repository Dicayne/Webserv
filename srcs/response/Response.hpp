/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:24:45 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/04 14:56:19 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../incs/webserv.hpp"
#include "../conf/serv_block.hpp"

class   Response
{
	private:
		Response(Response& toCopy);
		Response&	operator=(Response& toAssign);

		std::string							_response;
		std::map<std::string, std::string>	_stock;
		std::map<std::string, std::string>	_mime;
		std::map<int, std::string>			_messages;
		serv_block							*_block;

		/*	Attributes corresponding to the fields of the HTTP request:
		*/
		std::string	_protocol_version;
		std::string	_status;
		std::string	_status_message;
		std::string	_date;
		std::string	_server;
		std::string	_body;
		std::string	_content_type;
		std::string	_content_lenght; // Content-Lenght : ne doit pas dépasser le client body size max


	public:
		Response();
		Response(const std::string& protocol_version, int status, const std::string& url, serv_block *block);
		virtual ~Response();

		/*	All setters (one for each attribute corresponding to a field of the HTTP request):
		*/
		void		setProtocolVersion(const std::string& protocol_version);
		void		setStatus(int status);
		void		setStatusMessage(int status);
		void		setDate();
		void		setServer();
		void		setContentType(const std::string& content_type);
		void		setContentLenght();
		void		setBody(const std::string& body);

		void		buildMime(const std::string& key, const std::string& mapped);
		void		setMimeMap();
		void		buildMessages(int key, const std::string& mapped);
		void		setMessagesMap();

		void		buildPartResp(const std::string& key);
		void		buildResponse();
		void*		respond() const;

		/*	All getters (one for each attribute corresponding to a field of the HTTP request):
		*/
		const std::string&	getProtocolVersion() const;
		const std::string&	getStatus() const;
		const std::string&	getStatusMessage() const;
		const std::string&	getDate() const;
		const std::string&	getServer() const;
		const std::string&	getContentType() const;
		const std::string&	getContentLenght() const;
		const std::string&	getBody() const;

		const std::string&	getResponse() const;
};

std::ostream&		operator<<(std::ostream& os, const Response& r);

#endif
