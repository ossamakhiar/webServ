/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 18:54:09 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 14:46:59 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requestMessage.hpp"

requestMessage::requestMessage() : _presistent_con(true), _content_len(0)
{
	handling_state = READING_REQ;
	_setters_map.insert(std::pair<std::string, funcSetter>("Content-Type", &requestMessage::setContentType));
	_setters_map.insert(std::pair<std::string, funcSetter>("Host", &requestMessage::setHostname));
	_setters_map.insert(std::pair<std::string, funcSetter>("Connection", &requestMessage::setConnectionType));
}

requestMessage::requestMessage(const requestMessage& other)
{
	*this = other;
}

requestMessage& requestMessage::operator=(const requestMessage& rhs)
{
	if (this == &rhs)
		return (*this);
	_presistent_con = rhs._presistent_con;
	_content_len = rhs._content_len;
	handling_state = rhs.handling_state;
	return (*this);
}

requestMessage::~requestMessage()
{
	
}

// TODO :: Getters
int	requestMessage::getReqState(void) const
{
	return (handling_state);
}

// TODO :: Setters
void	requestMessage::setReqURI(const std::string& uri)
{
	// TODO :: check URI
	_URI = uri;
}

void	requestMessage::setMethod(const std::string& method)
{
	// TODO :: check method
	if  (!(method == "GET" || method == "POST" || method == "DELETE"))
		throw (METHOD_NOT_IMPLEMENTED);
	_method = method;
}

void	requestMessage::setHttpVersion(const std::string& version)
{
	// TODO :: check Http Version
	if (version != "HTTP/1.1")
		throw (HTTP_VERSION_NOT_SUPPORTED);
	_http_version = version;
}

void	requestMessage::setContentType(const std::string& t)
{
	_content_type = t;
}

void	requestMessage::setHostname(const std::string& host)
{
	_hostname = host;
}

void	requestMessage::setConnectionType(const std::string& type)
{
	_presistent_con = true;
	if (type == "close")
		_presistent_con = false;
}

// TODO :: REQUEST HANDLING {READING, PARSING..}

void	requestMessage::readReqMsg(int client_sock)
{
	int			bytes;
	char		buffer[BUFFER_MSG + 1] = {0};
	size_t		header_end;

	bytes = read(client_sock, buffer, BUFFER_MSG);
	if (bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
	if (bytes == 0)
		throw (ZERO_READED);
	buffer[bytes] = 0;
	_req_message.append(buffer, bytes);
	header_end = _req_message.find("\r\n\r\n");
	if (header_end != std::string::npos)
	{
		_req_header = _req_message.substr(0, header_end);
		_req_message = _req_message.substr(_req_header.length() + 4); // ! attention -- binary body
		std::cout << "\e[1;35m**********HEADER FOUND***********\e[0m"  << std::endl;
		//std::cout << _req_header << std::endl;
		handling_state = PARSING_REQ;
	}
}

size_t	requestMessage::request_line(void)
{
	size_t						pos = _req_header.find("\r\n");
	std::string					req_line = _req_header.substr(0, pos);
	std::vector<std::string>	tokens = Helpers::split(req_line, " ");

	if (tokens.size() != 3)
		throw (BAD_REQUEST);
	setMethod(tokens[0]);
	setReqURI(tokens[1]);
	setHttpVersion(tokens[2]);

	// std::cout << "REQUEST LINE: " << req_line << std::endl;
	return (pos + 2);
}

void	requestMessage::headerParsing(void)
{
	size_t 		i;
	int			state = KEY_FIELD;
	std::string	field_key;
	std::string	field_value;

	// ! Fuck rewrite this.....
	i = request_line();
	while (i < _req_header.length())
	{
		if (_req_header[i] == '\r' || _req_header[i] == '\n')
		{
			if (_req_header[i] == '\n' || _req_header[i + 1] != '\n')
			{
				throw (BAD_REQUEST);
			}
			i += 2;
			state = SET_FIELD;
		}
		if (_req_header[i] == ':' && state == KEY_FIELD \
			&& ++i && (state = VALUE_FIELD))
			continue ;
		if (state == KEY_FIELD)
			field_key.append(1, _req_header[i]);
		else if (state == VALUE_FIELD)
			field_value.append(1, _req_header[i]);
		else
		{
			if (_setters_map.count(field_key))
				(this->*(_setters_map[field_key]))(field_value);
			(field_key.clear(), field_value.clear()); // * clear the strings
			state = KEY_FIELD;
			continue ;
		}
		i++;
	}
}

void	requestMessage::requestHandling(int client_sock)
{
	if (handling_state == READING_REQ)
		readReqMsg(client_sock);
	if (handling_state == PARSING_REQ)
	{
		std::cout << "Body Readed in header reading: " << _req_message << std::endl;
		std::cout << _req_header << std::endl;
		std::cout << "client fd: " << client_sock << std::endl;
		headerParsing();
		// ! you shoould set the state to the Reading for reading the body
		// ** Check if the method that is the request used is POST then continue reading the body
		handling_state = DONE_REQ;
	}
}

// ************END************

std::ostream&	operator<<(std::ostream& os, const requestMessage& req)
{
	os << "\e[1;34mRequest:\e[0m\n";
	os << "Method: " << req._method <<std::endl;
	os << "URI: " << req._URI << std::endl;
	os << "HTTP Version: " << req._http_version << std::endl;
	os << "Persistent Connection: " << std::boolalpha << req._presistent_con << std::endl;
	os << "Host: " << req._hostname << std::endl;
	os << "Content-type: " << req._content_type << std::endl;

	return (os);
}