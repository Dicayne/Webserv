/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 11:16:47 by mabriand          #+#    #+#             */
/*   Updated: 2022/01/06 11:16:53 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../incs/webserv.hpp"
#include "../conf/serv_block.hpp"

class   Request
{
	private:
		Request();
		Request(Request& toCopy);
		Request&	operator=(Request& toAssign);

		int									_socket;
		serv_block							*_block;
		std::string							_request;
		std::map<std::string, std::string>	_stock;
		/*	Attributes corresponding to the fields of the HTTP request:
		*/
		std::string	_method;
		std::string	_url;
		std::string	_protocol_version;
		std::string	_host;
		std::string	_user_agent;
		std::string	_accept;
		std::string	_accept_language;
		std::string	_accept_encoding;
		std::string	_connection;
		std::string	_body;
		/*	Attributes corresponding to the parameters of the reponse's constructor that will be asked
			when creating it to send it back to the client:
		*/
		std::string	_response_protocol_version;
		int			_response_status_code;
		std::string	_response_url;

		bool connexion_end;
	public:
		Request(int socket, serv_block *block);
		~Request();
		/*	Some utils needed to extract information from the constructor's parameter 'buf' and to
			return some when the appropriate functions are called.
		*/
		std::string	extractInfo(std::string& line) const;// const std::string& --> pour le retour ?
		std::string	extractMapped(std::string& line) const;// const std::string& --> pour le retour ?
		void		buildMap(std::string& buf);
		void		parseBuf(std::string& buf);
		int			parse();
		/*	All setters (one for each attribute i.e. a field of the HTTP request):
		*/
		void		setMethod(std::string& buf);
		void		setUrl(std::string& buf);
		void		setProtocolVersion(std::string& buf);
		void		setHost();
		void		setUserAgent();
		void		setAccept();
		void		setAcceptLanguage();
		void		setAcceptEncoding();
		void		setConnection();
		void		setBody();
		/*
		*/
		void		defineProtocolVersion();
		void		defineStatusCode();
		void		defineUrl();
		/*	All getters (one for each attribute i.e. a field of the HTTP request):
		*/
		const std::string&	getMethod() const;
		const std::string&	getUrl() const;
		const std::string&	getProtocolVersion() const;
		const std::string&	getHost() const;
		const std::string&	getUserAgent() const;
		const std::string&	getAccept() const;
		const std::string&	getAcceptLanguage() const;
		const std::string&	getAcceptEncoding() const;
		const std::string&	getConnection() const;
		const std::string&	getBody() const;
		serv_block*	getBlock();
		/*
		*/
		const std::string&	returnProtocolVersion() const;
		int					returnStatusCode() const;
		const std::string&	returnUrl() const;
};

std::ostream&		operator<<(std::ostream& os, const Request& r);

#endif
