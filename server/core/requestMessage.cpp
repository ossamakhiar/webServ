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
		_chunked(false),
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




// TODO :: REQUEST HANDLING {READING, PARSING, BODY..}

// ************* BODY HANDLING **************
void	requestMessage::openAndWrite(const char* data, size_t size, bool creation_flag)
{
	if (creation_flag)
	{
		body_fd = open((std::string("post/") + Helpers::randomFileNameGen()).c_str(), O_WRONLY | O_CREAT, 0666);
		if (body_fd == -1)
			throw (INTERNAL_SERVER_ERROR);
	}
	if (write(body_fd, data, size) == -1) // ! what the fuck 30ms of diffrence
		throw (INTERNAL_SERVER_ERROR);
}

void	requestMessage::chunked_approach(const char *buffer, int bytes, bool creation_flag)
{
	TE_reader.bufferFeed(buffer, bytes);
	TE_reader.processAndShiftData();
	// ! getbuffer should return all the chunks that is full...
	openAndWrite((const char *)TE_reader.getChunks(), TE_reader.chunk_size(), creation_flag);
	// ? I'm Stuck here....
	TE_reader.clear_data();
	if (TE_reader.isLastChunk())
		handling_state = DONE_REQ;
}

void	requestMessage::extractBodyContent(char *buffer, int bytes)
{
	if (_chunked)
	{
		// ? transfer encoding chunked..
		chunked_approach(buffer, bytes);
		return ;
	}
	// ? Store-and-forward approach
	// ! why i store it in a container why just write it
	for (size_t i = 0; i < static_cast<size_t>(bytes) && _content_len > 0; ++i, --_content_len)
		_req_body.push_back(buffer[i]);
	handling_state = ((!_content_len) ? DONE_REQ : handling_state);
	openAndWrite((const char*)_req_body.data(), _req_body.size());
	_req_body.clear(); // ! clear the body that is written on the file
}

void	requestMessage::handleBodyRead(void)
{
	if (_chunked)
	{
		// ? set state to reading body, but maybe change in case we found last chunk
		handling_state = READING_BODY_REQ;
		chunked_approach((const char*)_req_body.data(), _req_body.size(), true);
		_req_body.clear();
		return ;
	}
	if (_content_len >= _vs->getMaxBodySize())
		throw (REQUEST_ENTITY_TOO_LARGE);
	openAndWrite((const char*)_req_body.data(), _req_body.size(), true);
	_content_len -= _req_body.size();
	handling_state = (!_content_len ? DONE_REQ : READING_BODY_REQ);
	_req_body.clear();
}

// ***************** END BODY HANDLING *********************





// **************** HEADER HANDLING ************************
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
	if (_header_fields.count("Transfer-Encoding"))
	{
		if (_header_fields["Transfer-Encoding"] != "chunked")
			throw (METHOD_NOT_IMPLEMENTED);
		_chunked = true;
	}
	if (_header_fields.count("Content-Length"))
	{
		if (_chunked)
			throw (BAD_REQUEST);
		setContentLen(_header_fields["Content-Length"]);
	}
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
	(setImportantFields(), setProperVS());
	if (_method == "POST" && !_header_fields.count("Content-Length") && !_header_fields.count("Transfer-Encoding"))
		throw (BAD_REQUEST);
}

// **************** END HEADER HANDLING ************************


void	requestMessage::print_body()
{
	std::cout << "Body Readed in header reading: " << _req_body.size() << std::endl;
	for (size_t i = 0; i < _req_body.size(); i++)
		std::cout << static_cast<int>(_req_body[i]) << " ";
	std::cout << "**************************************************" << std::endl;
}

void	requestMessage::readReqMsg(int client_sock)
{
	int			bytes;
	char		buffer[BUFFER_MSG + 1] = {0};

	bytes = read(client_sock, buffer, BUFFER_MSG);
	if (bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
	if (bytes == 0)
		throw (ZERO_READED);
	buffer[bytes] = 0;
	if (handling_state == READING_BODY_REQ)
	{
		extractBodyContent(buffer, bytes);
		return ;
	}
	headerExtracting(buffer, bytes);
}

// ! read body
void	requestMessage::requestHandling(int client_sock)
{
	if (handling_state == READING_REQ || handling_state == READING_BODY_REQ)
		readReqMsg(client_sock);
	if (handling_state == PARSING_REQ)
	{
		// print_body();
		std::cout << _req_header << std::endl;
		std::cout << "client fd: " << client_sock << std::endl;
		headerParsing();
		// ! you shoould set the state to the Reading for reading the body
		// ** Check if the method that is the request used is POST then continue reading the body
		handling_state = DONE_REQ;
		if (_method == "POST")
			handleBodyRead(); // ? handling body that read during header reading
	}
	if (handling_state == DONE_REQ)
		close(body_fd);
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