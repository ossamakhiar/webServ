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
#include <unistd.h>
#include <fcntl.h>
#include "helpers.hpp"
#include "virtualServer.hpp"
#include "bufferQueuing.hpp"

# define BUFFER_MSG 500
# define MAX_REQ_URI 8192

enum e_handling_states
{
	READING_REQ,
	READING_BODY_REQ,
	PARSING_REQ,
	DONE_REQ
};

enum e_status_code
{
	OK = 200,
	BAD_REQUEST = 400,
	REQUEST_ENTITY_TOO_LARGE = 413,
	REQUEST_URI_TOO_LONG = 414,
	INTERNAL_SERVER_ERROR = 500,
	METHOD_NOT_IMPLEMENTED = 501,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	// ! Maybe This Shouldn't be here
	ZERO_READED = -1
};


class requestMessage
{
private:
	int body_fd; // ? file exist in case of POST body

	int					handling_state;
	std::string			_req_message; // ** store the request message
	std::string			_req_header;
	std::vector<char>	_req_body;

	// ? To define the server that client want to interact with.
	std::vector<virtualServer*> &_vs_endpoint;
	virtualServer		*&_vs;
	// ? ******************************

	bool			_chunked;// ? transfer encoding body content case
	bufferQueuing	TE_reader;

	std::map<std::string, std::string> _header_fields;

	bool		_presistent_con;
	int			_content_len; // ! mandatory in case request has a body message
	std::string	_hostname; // ! make it mandatory :)
	std::string	_method, _URI, _http_version;


	// ** private members function
	size_t	request_line(void);
	void	readReqMsg(int client_sock);
	void	headerParsing(void);
	void	headerExtracting(char *, int);

	void	openAndWrite(const char* data, size_t size, bool creation_flag = false);
	// void	isBodyComplete(void);

	void	chunked_approach(const char *buffer, int bytes, bool c = false);
	void	extractBodyContent(char *buffer, int bytes);
	void	handleBodyRead(void);

	requestMessage(const requestMessage&);
	requestMessage& operator=(const requestMessage&);

public:
	requestMessage(std::vector<virtualServer*>&, virtualServer *&);
	~requestMessage();

	// * Setters
	void	setReqURI(const std::string&);
	void	setMethod(const std::string&);
	void	setHttpVersion(const std::string&);
	void	setConnectionType(const std::string&);
	void	setContentLen(const std::string&);

	void	setProperVS(void);
	void	setImportantFields(void);

	// * Getters
	int	getReqState(void) const;

	void	requestHandling(int client_sock);

	// ! remove this
	void	print_body();
	friend std::ostream&	operator<<(std::ostream& os, const requestMessage& req);
};

std::ostream&	operator<<(std::ostream& os, const requestMessage& req);

#endif