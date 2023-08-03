/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:37:42 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 14:12:16 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTMESSAGE_HPP
# define REQUESTMESSAGE_HPP

#include <map>
#include "helpers.hpp"
#include <unistd.h>

# define BUFFER_MSG 400

class requestMessage;

enum e_handling_states
{
	READING_REQ,
	PARSING_REQ,
	DONE_REQ
};

enum e_status_code
{
	OK = 200,
	BAD_REQUEST = 400,
	INTERNAL_SERVER_ERROR = 500,
	METHOD_NOT_IMPLEMENTED = 501,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	// ! Maybe This Shouldn't be here
	ZERO_READED = -1
};

enum e_field_states
{
	KEY_FIELD,
	VALUE_FIELD,
	SET_FIELD
};

typedef void (requestMessage::*funcSetter)(const std::string&);

class requestMessage
{
private:
	std::map<std::string, funcSetter>	_setters_map;

	int			handling_state;
	std::string	_req_message; // ** store the request message
	std::string	_req_header;
	std::string	_req_body;

	std::map<std::string, std::string> _header_fields;

	bool		_presistent_con;
	int			_content_len; // ! mandatory in case request has a body message
	std::string	_hostname; // ! make it mandatory :)
	std::string	 _content_type;
	std::string	_method;
	std::string	_URI;
	std::string	_http_version;


	// ** private members function
	size_t	request_line(void);

public:
	requestMessage();
	~requestMessage();
	requestMessage(const requestMessage&);
	requestMessage& operator=(const requestMessage&);

	// * Setters
	void	setReqURI(const std::string&);
	void	setMethod(const std::string&);
	void	setHttpVersion(const std::string&);
	void	setContentType(const std::string&);
	void	setHostname(const std::string&);
	void	setConnectionType(const std::string&);


	// * Getters
	int	getReqState(void) const;

	// void	fillRequestMessage(const std::string& req);
	void	requestHandling(int client_sock);
	void	readReqMsg(int client_sock);
	void	headerParsing(void);

	// ! remove this
	friend std::ostream&	operator<<(std::ostream& os, const requestMessage& req);
};

std::ostream&	operator<<(std::ostream& os, const requestMessage& req);

#endif
