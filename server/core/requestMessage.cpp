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

requestMessage::requestMessage(std::vector<virtualServer*> &ends_v, virtualServer *&vs) : _vs_endpoint(ends_v), \
		_vs(vs), \
		_presistent_con(true), \
		_content_len(0)
{
	handling_state = READING_REQ;
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
	size_t	i = 0;

	while (i < uri.length())
	{
		if (uri[i] != '%')
		{
			_URI += uri[i++];
			continue ;
		}
		i++;
		_URI += Helpers::hexaToDecimal(Helpers::strTolower(uri.substr(i, 2)));
		i += 2;
	}
}

void	requestMessage::setMethod(const std::string& method)
{
	if  (!(method == "GET" || method == "POST" || method == "DELETE"))
		throw (METHOD_NOT_IMPLEMENTED);
	_method = method;
}

void	requestMessage::setHttpVersion(const std::string& version)
{
	if (version != "HTTP/1.1")
		throw (HTTP_VERSION_NOT_SUPPORTED);
	_http_version = version;
}

void	requestMessage::setConnectionType(const std::string& type)
{
	_presistent_con = true;
	if (type == "close")
		_presistent_con = false;
}

void	requestMessage::setContentLen(const std::string& len)
{
	try
	{
		_content_len = Helpers::safeAtoi(len);
	} catch (std::exception& e) {
		throw (BAD_REQUEST);
	}
}

void	requestMessage::setProperVS(void)
{
	for (std::vector<virtualServer*>::iterator it = _vs_endpoint.begin(); it != _vs_endpoint.end(); ++it)
		if ((*it)->getEndpoint().first == _hostname.substr(0, _hostname.find(':')))
			_vs = (*it);
}

void	requestMessage::extractBodyContent(char *buffer, int bytes)
{
	std::cout << (void *)buffer << std::endl;
	(void)bytes;
	handling_state = DONE_REQ;
}

// TODO :: REQUEST HANDLING {READING, PARSING..}

void	requestMessage::headerExtracting(char *buffer, int bytes)
{
	int			tmp;
	size_t		header_end;

	tmp = _req_message.length();
	_req_message.append(buffer, bytes);
	header_end = _req_message.find("\r\n\r\n");
	if (header_end != std::string::npos)
	{
		_req_header = _req_message.substr(0, header_end + 2);
		// * Store the part of body that is read in vector
		for (size_t i = 0; i < bytes - ((_req_header.length() + 2) - tmp); i++) // !binary body
			_req_body.push_back(_req_message[i + (_req_header.length() + 2)]);
		handling_state = PARSING_REQ;
	}
}

void	requestMessage::readReqMsg(int client_sock)
{
	int			bytes;
	char		buffer[BUFFER_MSG + 1] = {0};

	std::cout << "Reading......." << std::endl;
	bytes = read(client_sock, buffer, BUFFER_MSG);
	if (bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
	if (bytes == 0)
		throw (ZERO_READED);
	// ! extract Body if header parsing finished...
	buffer[bytes] = 0;
	if (handling_state == READING_BODY_REQ)
	{
		extractBodyContent(buffer, bytes);
		return ;
	}
	headerExtracting(buffer, bytes);
}

size_t	requestMessage::request_line(void)
{
	size_t						pos = _req_header.find("\r\n");
	std::string					req_line = _req_header.substr(0, pos);
	std::vector<std::string>	tokens = Helpers::split(req_line, " ");

	if (tokens.size() != 3)
		throw (BAD_REQUEST);
	setMethod(tokens[0]);
	if (tokens[1].length() > MAX_REQ_URI)
		throw (REQUEST_URI_TOO_LONG);
	setReqURI(tokens[1]);
	setHttpVersion(tokens[2]);
	return (pos + 2);
}

void	requestMessage::setImportantFields(void)
{
	if (_header_fields.count("Connection"))
		setConnectionType(_header_fields["Connection"]);
	if (_header_fields.count("Content-Length"))
		setContentLen(_header_fields["Content-Length"]);
	if (!_header_fields.count("Host"))
		throw (BAD_REQUEST);
	_hostname = _header_fields["Host"];
}

void	requestMessage::headerParsing(void)
{
	size_t		i = 0, pos, colon_pos;
	std::string	field_key;
	std::string	field_value;

	i = request_line();
	while (i < _req_header.length())
	{
		colon_pos = _req_header.find_first_of(":", i);
		pos = _req_header.find("\r\n", i);
		field_key = _req_header.substr(i, colon_pos - i);
		field_value = Helpers::trim(_req_header.substr(colon_pos + 1, pos - colon_pos - 1));
		_header_fields[field_key] = field_value;
		i = pos + 2;
	}
	setImportantFields();
	setProperVS();
	if (_method == "POST" && !_header_fields.count("Content-Length"))
		throw (BAD_REQUEST);
}

void	requestMessage::print_body()
{
	std::cout << "Body Readed in header reading: " << _req_body.size() << std::endl;
	for (size_t i = 0; i < _req_body.size(); i++)
		std::cout << static_cast<int>(_req_body[i]) << " ";
	std::cout << "**************************************************" << std::endl;
}

void	requestMessage::openAndWrite(void)
{
	int	fd;

	fd = open("file.ext", O_APPEND | O_WRONLY | O_CREAT);
	for (std::vector<unsigned char>::iterator it = _req_body.begin(); it != _req_body.end(); ++it)
	{
		if (write(fd, &(*it), 1) == -1)
			throw (INTERNAL_SERVER_ERROR);
	}
	close(fd);
}

void	requestMessage::isBodyComplete(void)
{
	openAndWrite(); // ! Write the body on a file
	if (_req_body.size() == static_cast<size_t>(_content_len))
		handling_state = DONE_REQ;
	else
		handling_state = READING_BODY_REQ;
	_req_body.clear();
}

// ! read body
void	requestMessage::requestHandling(int client_sock)
{
	if (handling_state == READING_REQ || handling_state == READING_BODY_REQ)
		readReqMsg(client_sock);
	if (handling_state == PARSING_REQ)
	{
		print_body();
		std::cout << _req_header << std::endl;
		std::cout << "client fd: " << client_sock << std::endl;
		headerParsing();
		// ! you shoould set the state to the Reading for reading the body
		// ** Check if the method that is the request used is POST then continue reading the body
		if (_method == "POST")
			isBodyComplete();
		else
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
	for (std::map<std::string, std::string>::const_iterator it = req._header_fields.begin(); it != req._header_fields.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	return (os);
}