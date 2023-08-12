/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:06:53 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/12 16:51:56 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _client_socket(-1), _error_body(false), \
	_vs(NULL), _location(NULL)
{
	_cgi_exists = false;
	_client_socket = -1;
	_body_fd = -1;
	_req = NULL;
	_responsing_state = BODY_PRODUCING;
	_stations[BODY_PRODUCING] = &Response::bodyProdcucers;

	_methods_handler["GET"] = &Response::getHandler;

	_html_start = "<html>\n<head>\n<title>page--</title><head>\n<body>";
	_html_close = "</body>\n</html>";
	fillDefaultErrorPages();
	fillReasonPhrases();
}

Response::~Response()
{
	
}

Response::Response(const Response& other)
{
	_error_pages = other._error_pages;
}

Response&	Response::operator=(const Response& rhs)
{
	if (this == &rhs)
		return (*this);
	return (*this);
}

// TODO ::: Setters
void	Response::setVS(virtualServer* vs)
{
	_vs = vs;
}

void	Response::setLocation(locationBlock* loc)
{
	_location = loc;
}

void	Response::setRequest(requestMessage *req)
{
	_req = req;
}

void	Response::setStatusCode(int status_code)
{
	_status_code = status_code;
}

void	Response::setRequestMethod(const std::string& method)
{
	_request_method = method;
}

// TODO :: Getters
int	Response::getResponseState(void) const
{
	return (_responsing_state);
}

// * Helpers

void	Response::fillReasonPhrases(void)
{
	_reason_phrase[OK] = "Ok";
	_reason_phrase[MOVED_PERMANENTLY] = "Moved Permanently";
	_reason_phrase[FORBIDDEN] = "Forbidden";
	_reason_phrase[NOT_FOUND] = "Not Found";
}

void Response::fillDefaultErrorPages(void)
{
	_error_pages[MOVED_PERMANENTLY] = "<html><body><h1>301 Moved Permanently</h1><hr><p>The resource has been moved permanently.</p></body></html>";
    _error_pages[BAD_REQUEST] = "<html><body><h1>400 Bad Request</h1><hr><p>Your request was invalid.</p></body></html>";
    _error_pages[401] = "<html><body><h1>401 Unauthorized</h1><hr><p>You are not authorized to access this resource.</p></body></html>";
    _error_pages[FORBIDDEN] = "<html><body><h1>403 Forbidden</h1><hr><p>You don't have permission to access this resource.</p></body></html>";
    _error_pages[NOT_FOUND] = "<html><body><h1>404 Not Found</h1><hr><p>The requested page was not found.</p></body></html>";
	_error_pages[METHOD_NOT_ALLOWED] = "<html><body><h1>405 Method Not Allowed</h1><hr><p>The requested method is not allowed for this resource.</p></body></html>";
    _error_pages[INTERNAL_SERVER_ERROR] = "<html><body><h1>500 Internal Server Error</h1><hr><p>An internal server error occurred.</p></body></html>";
}

std::string	Response::validateRootPath(const std::string& requested_path)
{
	std::string	path;

	// ** case of there no location match the requested path, join it with the root of the server
	path = (_location != NULL) ? _location->getRoot() : _vs->getRootDir();
	if (path[path.length() - 1] == '/' && requested_path[0] == '/')
		path.erase(path.length() - 1);
	path += requested_path;
	return (path);
}






// *************** Directory requested serving ***************

std::string	Response::checkIndex(const std::string& path)
{
	// * check all the available index by the location
	std::string	index_path;
	const std::vector<std::string>	indexs = _location->getIndexs();

	for (size_t i = 0; i < indexs.size(); ++i)
	{
		index_path = path;
		std::cout << "\e[1;31mIndex: " << indexs[i] << std::endl;
		if (index_path[index_path.length() - 1] != '/')
			index_path += "/";
		index_path += indexs[i];
		std::cout << "index path: \e[0m" << index_path << std::endl;
		if (PathVerifier::path_exists(index_path)) // ! maybe you check also if it's a file
			return (index_path);
	}
	return ("");
}

void	Response::directoryListing(const std::string& path)
{
	DIR				*dir_ptr;
	struct dirent	*dir_entity;
	std::string		html_doc = _html_start + "<h1>Director listing for " + path + "</h1>\n<hr>\n<ul>\n";

	// * list the content of the directory (path)
	if (_location && !_location->get_autoindex()) // ! if loactioon exist, and autoindex is off so FORBEDDIN
		throw (FORBIDDEN);
	dir_ptr = opendir(path.c_str());
	while ((dir_entity = readdir(dir_ptr)) != NULL)
	{
		// ** get name of the entity and maybe type also
		html_doc += "<li><a href=\"" + std::string(dir_entity->d_name);
		if (dir_entity->d_type == DT_DIR)
			html_doc += "/";
		html_doc += "\">" + std::string(dir_entity->d_name) + "</a></li>\n";
	}
	html_doc += _html_close;
	_body = html_doc;
	closedir(dir_ptr);
}

void	Response::directoryServing(const std::string& path)
{
	std::string		index_path, line;
	std::ifstream	index_file;

	// ** check index
	if (!(index_path = checkIndex(path)).empty())
	{
		// ! serve the index page..
		// ? open path and fill the body
		index_file.open(index_path.c_str(), std::ios::in);
		if (!index_file.good())
			throw (INTERNAL_SERVER_ERROR);
		while (std::getline(index_file, line))
		{
			_body += line;
			if (!index_file.eof())
				_body += "\n";
		}
		index_file.close();
		return ;
	}
	directoryListing(path);
}

// **************** Respond - Directory requested [END] *************






// ** RESPONSING Sending....

void	Response::responseHeader(void)
{
	std::string	status_line;

	status_line = "HTTP/1.1 " + std::to_string(_status_code) + " " + _reason_phrase[_status_code] + "\r\n";
	_headers = status_line;
	if (_status_code == MOVED_PERMANENTLY)
		_headers += "Location: " + _redirection_path + "\r\n";
	_headers += "Connection: keep-alive\r\n";
	_headers += "Content-Length: " + std::to_string(_body.length());
	_headers += "\r\n\r\n";
}

void	Response::reposnseSending(void)
{
	int bytes = 0;

	if ((bytes = write(_client_socket, _body.c_str(), _content_len)) == -1)
	{
		perror("sending");
		throw (INTERNAL_SERVER_ERROR);
	}

	if (bytes == 0)
		throw (-1);
	std::cout << "Bytes.: " << bytes  << " " << _content_len << std::endl;
	_content_len -= bytes;
	if (_content_len <= 0)
		_responsing_state = RESPONSE_DONE;
	_body = _body.substr(bytes);
}

void	Response::respond(void)
{
	// ! header making

	responseHeader();
	std::cout << "header: " << _headers << "\n";

	// ! Sending state setting
	if (write(_client_socket, _headers.c_str(), _headers.size()) < 0)
        throw (INTERNAL_SERVER_ERROR);
	_content_len = _body.length();
	_responsing_state = RESPONSE_SENDING;
}

// **** END RESPONSING ****

// ** method handlers
void	Response::getHandler(void) // ? GET Request handler...
{
	std::string	path;

	path = validateRootPath(_req->getPath());
	std::cout  << "\e[1;32mPath: \e[0m" << path << std::endl;
	if (!PathVerifier::path_exists(path))
		throw (NOT_FOUND);
	if (access(path.c_str(), F_OK | R_OK))
		throw (FORBIDDEN);
	if (PathVerifier::is_directory(path))
	{
		if (_req->getPath()[_req->getPath().length() - 1] != '/')
		{
			_redirection_path = _req->getPath() + "/";
			throw (MOVED_PERMANENTLY);
		}
		directoryServing(path); // ** server index, or directory listing on case of autoindex
		return ;
	}
	if (_cgi_exists)
	{
		// set path to cgi output
	}
	fileServing(path);
}



// ************* BODY PRODUCERS *********

void	Response::checkErrorCode(int status_code) // ? Body producer
{
	//_error_body = true; // ? why this maybe i'll never neeed it
	if (_error_pages.count(status_code))
		_body = _error_pages[status_code];
	respond(); // you should respond here because, it called in more than one case
}

void	Response::fileServing(const std::string& path)
{
	std::ifstream	file;
	std::string		line;
	// std::streamsize	size;

	file.open(path.c_str(), std::ios::in);
	if (!file.good())
		throw (INTERNAL_SERVER_ERROR);
	while (std::getline(file, line))
	{
		_body += line;
		if (!file.eof())
			_body += '\n';
	}
	file.close();
}

void	Response::bodyProdcucers(void)
{
	if (_status_code != OK)
	{
		checkErrorCode(_status_code);// ! why i pass _status_code?
		return ;
	}
	// ! check if a cgi and handle it
	// if (_cgi)
	// {
	// 	// do blah blah
	// } 

	(this->*_methods_handler[_request_method])();
	respond();
}






// ********

void	Response::buildResponse(int client_socket)
{
	// if (_client_socket == -1)
	_client_socket = client_socket;

	std::cout << "\e[1;33mbuilding response call..................\e[0m" << client_socket << std::endl;
	try {
		if (_responsing_state == BODY_PRODUCING)
			bodyProdcucers();
		else if (_responsing_state == RESPONSE_SENDING)
			reposnseSending();
	} catch (e_status_code code) {
		_status_code = code;
		checkErrorCode(_status_code);
		reposnseSending();
		_responsing_state = RESPONSE_DONE;
	}
	
	// ** checking of status code, if is it not ok, respond with the associated page error
	// checkErrorCode(status_code);
	// ! if the body empty so there is no error, and check the requested URI
	// if (!_error_body)
	// 	requestedPathServe(req);

	// for (size_t i = 0; i < _body.size(); ++i)
	// 	std::cout << _body[i];
	// std::cout << _body << std::endl;
	// std::string responseHeaders = "HTTP/1.1 200 OK\r\n";
    // responseHeaders += "Server: oussama khiar\r\n";
    // responseHeaders += "Content-Length: " + std::to_string(_body.length()) + "\r\n";
    // responseHeaders += "\r\n";

	// std::cout << "client-socket: " << client_socket << std::endl;
    // if (write(client_socket, responseHeaders.c_str(), responseHeaders.size()) < 0) {
    //     throw std::runtime_error("Error writing response headers");
    // }

	// if (write(client_socket, _body.c_str(), _body.length()) == -1)
	// 	throw std::runtime_error("SERVER DOWN");
	// ! header construction 
}


// *************** END *****************128