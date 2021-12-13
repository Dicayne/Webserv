/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Request.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mabriand <mabriand@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2021/12/06 19:46:24 by mabriand		  #+#	#+#			 */
/*   Updated: 2021/12/09 13:24:56 by mabriand		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../incs/webserv.hpp"

class   Request
{
	private:
		Request();
		Request(Request& toCopy);
		Request&	operator=(Request& toAssign);

		std::string							request;
		std::map<std::string, std::string>	stock;
		/*
			Attributes corresponding to the fields of the HTTP request:
		*/
		std::string	method;
		std::string url;
		std::string protocol_version;
		std::string host;
		std::string user_agent;
		std::string accept;
		std::string accept_language;
		std::string accept_encoding;
		std::string connection;
		std::string body;

	public:
		Request(std::string& buf);
		~Request();

		std::string	extractInfo(std::string& line) const;// const std::string& --> pour le retour ?
		std::string	extractMapped(std::string& line) const;// const std::string& --> pour le retour ?
		void		buildMap(std::string& buf);
		/*
			All setters (one for each attribute corresponding to a field of the HTTP request):
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
			All getters (one for each attribute corresponding to a field of the HTTP request):
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

};

std::ostream&	operator<<(std::ostream& os, const Request& r);

#endif