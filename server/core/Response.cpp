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
	_cgi_ext = "";
	_client_socket = -1;
	_body_fd = -1;
	_req = NULL;
	_content_len = 0;
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

// void	Response::setPhysicalPath(const std::)

// TODO :: Getters
int	Response::getResponseState(void) const
{
	return (_responsing_state);
}



// ************* Helpers ***************

void	Response::fillReasonPhrases(void)
{
	_reason_phrase[OK] = "Ok";
	_reason_phrase[MOVED_PERMANENTLY] = "Moved Permanently";
	_reason_phrase[FORBIDDEN] = "Forbidden";
	_reason_phrase[BAD_REQUEST] = "Bad Request";
	_reason_phrase[NOT_FOUND] = "Not Found";
	_reason_phrase[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	_reason_phrase[INTERNAL_SERVER_ERROR] = "Internal Server Error";
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

void	Response::setContentLength(void)
{
	int		read_bytes;
	char	buffer[READ_BUFFER + 1];

	if (_stored_type == RAM_BUFFER)
	{
		_content_len = _body.length();
		return ;
	}

	while ((read_bytes = read(_body_fd, buffer, READ_BUFFER)) > 0)
		_content_len += read_bytes;
	if (read_bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
	lseek(_body_fd, (-1 * _content_len), SEEK_CUR);
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
	std::string		html_doc = _html_start + "<h1>Directory listing for " + path + "</h1>\n<hr>\n<ul>\n";

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

	status_line = "HTTP/1.1 " + Helpers::to_string(_status_code) + " " + _reason_phrase[_status_code] + "\r\n";
	_headers = status_line;
	if (_status_code == MOVED_PERMANENTLY)
		_headers += "Location: " + _redirection_path + "\r\n";
	_headers += "Connection: keep-alive\r\n";
	_headers += "Content-Length: " + Helpers::to_string(_content_len);
	_headers += "\r\n\r\n";
	std::cout << "\e[1;32mLength: \e[0m" << _content_len << std::endl;
}

void	Response::body_buffer_sending(void)
{
	int	bytes;

	bytes = write(_client_socket, _body.c_str(), _body.length());
	if (bytes <= 0)
		throw (INTERNAL_SERVER_ERROR);
}

void	Response::file_chunks_sending(void)
{
	int		read_bytes;
	int		write_bytes;
	char	buffer[READ_BUFFER + 1];


	read_bytes = read(_body_fd, buffer, READ_BUFFER);

	if (read_bytes == 0)
	{
		_responsing_state = RESPONSE_DONE;
		return ;
	}
	if (read_bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
	write_bytes = write(_client_socket, buffer, read_bytes);
	if (write_bytes == 0)
		throw (-1);
	if (write_bytes == -1)
		throw (INTERNAL_SERVER_ERROR);
}

void	Response::reponseSending(void)
{
	// int bytes = 0;

	std::cout << "sending....\n";
	if (_stored_type == RAM_BUFFER)
	{
		body_buffer_sending();
		// ? set response state to DONE cause, buffered body is written at once
		_responsing_state = RESPONSE_DONE;
		return ;
	}
	file_chunks_sending();
}

void	Response::respond(void)
{
	// ! header making
	setContentLength();
	responseHeader();
	std::cout << "header: " << _headers << "\n";

	// ! Sending state setting
	if (write(_client_socket, _headers.c_str(), _headers.size()) < 0)
        throw (INTERNAL_SERVER_ERROR);
	_content_len = _body.length();
	_responsing_state = RESPONSE_SENDING;
}

// **** END RESPONSING ****

// ** METHOD handlers
void	Response::getHandler(void) // ? GET Request handler...
{
	std::string	rooted_path;

	rooted_path = _req->getPhysicalPath();
	if (_cgi_exists)
	{
		// set path to cgi output
		// cgi_handler();
		// rooted_path = _cgi_output;
		// return ;
	}
	// fileServing(_req->getPhysicalPath());
	// maybe here i should check the cgi maybe, maybe...
	_body_fd = open(rooted_path.c_str(), O_RDONLY);

	if (_body_fd == -1)
		throw (INTERNAL_SERVER_ERROR);
	// ? in case of extrnal file set the body type to external storage type
	_stored_type = EXTERNAL_STORAGE;
}


void	Response::postHandler(void)
{
	// POST handler...
	if (!_cgi_exists)
		throw (FORBIDDEN);
	
}


// ************* BODY PRODUCERS *********

char	**cgi_env_setting(void)
{
	char	**env = NULL;

	// cgi Env setting

	return (env);
}

void	Response::cgi_handler(void)
{
	int			pid;
	char		**cgi_env;
	const char	*cgi_args[3];

	cgi_env = cgi_env_setting();
	cgi_args[0] = _location->getCGI().at(_cgi_ext).c_str();
	cgi_args[1] = _req->getPhysicalPath().c_str();
	cgi_args[2] = NULL;

	pid = fork();
	if (!pid)
	{
		if (_request_method == "POST")
		{
			// dup2 cgi input to the post body file
		}
		execve(cgi_args[0], cgi_args, cgi_env);
		exit(EXIT_FAILURE);
	}
}

void	Response::checkErrorCode(int status_code) // ? Body producer
{
	//_error_body = true; // ? why this maybe i'll never neeed it
	if (_error_pages.count(status_code))
		_body = _error_pages[status_code];
	// ? set the type of body to Buffer type
	_stored_type = RAM_BUFFER;
	respond(); // you should respond here because, it called in more than one case
}

void	Response::DirectoryRequestedChecking(const std::string& path)
{
	if (_req->getPath()[_req->getPath().length() - 1] != '/')
	{
		if (_request_method == "DELETE")
			throw (CONFLICT);
		_redirection_path = _req->getPath() + "/";
		throw (MOVED_PERMANENTLY);
	}
	directoryServing(path); // ** server index, or directory listing on case of autoindex
	// ? set type of body to Buffer type
	_stored_type = RAM_BUFFER;
	respond();
}



bool	Response::checkCgiExistence(void)
{
	std::string	file_ext;

	file_ext = PathVerifier::get_file_ext(_req->getPath());
	if (file_ext.empty() || _location->getCGI().count(file_ext))
		return (false);
	_cgi_ext = file_ext;
	return (true);
}

void	Response::bodyProdcucers(void)
{
	std::string	path;

	if (_status_code != OK)
	{
		checkErrorCode(_status_code);// ! why i pass _status_code?
		return ;
	}

	path = _req->getPhysicalPath();
	std::cout  << "\e[1;32mPath: \e[0m" << path << std::endl;
	if (!PathVerifier::path_exists(path))
		throw (NOT_FOUND);
	if (access(path.c_str(), F_OK | R_OK))
		throw (FORBIDDEN);
	if (PathVerifier::is_directory(path))
	{
		DirectoryRequestedChecking(path);
		return ;
	}

	// ! check if a cgi and handle it
	_cgi_exists = checkCgiExistence();

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
			reponseSending();
	} catch (e_status_code code) {
		_status_code = code;
		checkErrorCode(_status_code);
		reponseSending();
		_responsing_state = RESPONSE_DONE;
	}
}


// *************** END *****************128