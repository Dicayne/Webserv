/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:46:24 by mabriand          #+#    #+#             */
/*   Updated: 2021/12/08 15:05:45 by mabriand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "webserv.hpp"

class   Request
{
    private:
        Request();
        Request(Request& toCopy);
        Request&    operator=(Request& toAssign);
        
		std::string	request;

        std::string method;
        std::string url;
        std::string protocol_version;

		std::map<std::string, std::string>	info;

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

        std::string     getInfo(std::string& line) const;// const std::string& --> pour le retour ?
		std::string     getMapped(std::string& line) const;// const std::string& --> pour le retour ?
		
        void			buildMap(std::string& buf);

        void		    setMethod(std::string& buf);
		void		    setUrl(std::string& buf);
		void			setProtocolVersion(std::string& buf);
        void			setHost();
        void			setUserAgent();
        void			setAccept();
        void			setAcceptLanguage();
        void			setAcceptEncoding();
        void			setConnection();
        void			setBody();

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