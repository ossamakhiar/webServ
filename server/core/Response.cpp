/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:06:53 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/10 17:30:56 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _error_body(false), \
	_vs(NULL), _location(NULL)
{
	fillDefaultErrorPages();
	// _error_pages.insert(std::pair<int, std::string>(BAD_REQUEST, "<html><head></head><body><h1>400 bad request</h1></body></html>"))
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


// * Helpers
void Response::fillDefaultErrorPages(void)
{
    _error_pages[BAD_REQUEST] = "<html><body><h1>400 Bad Request</h1><hr><p>Your request was invalid.</p></body></html>";
    _error_pages[401] = "<html><body><h1>401 Unauthorized</h1><hr><p>You are not authorized to access this resource.</p></body></html>";
    _error_pages[403] = "<html><body><h1>403 Forbidden</h1><hr><p>You don't have permission to access this resource.</p></body></html>";
    _error_pages[NOT_FOUND] = "<html><body><h1>404 Not Found</h1><hr><p>The requested page was not found.</p></body></html>";
    _error_pages[INTERNAL_SERVER_ERROR] = "<html><body><h1>500 Internal Server Error</h1><hr><p>An internal server error occurred.</p></body></html>";
}


// ** RESPONSING....

void	Response::checkErrorCode(int status_code) // ? Body producer
{
	if (status_code == OK)
		return ;
	_error_body = true;
		for (size_t i = 0; i < _error_pages[status_code].length(); ++i)
		_body.push_back(_error_pages[status_code][i]);
	// _body = _error_pages[status_code];
}

std::string	Response::validateRootPath(const std::string& requested_path)
{
	std::string	path;

	// if (_location)
	// {
	// 	path += _location->getRoot();
	// 	if (path[path.length() - 1] == '/' && requested_path[0] == '/')
	// 		path.erase(path.length() - 1);
	// 	path += requested_path;
	// 	return (path);
	// }
	// ** case of there no location match the requested path, join it with the root of the server
	path = (_location != NULL) ? _location->getRoot() : _vs->getRootDir();
	if (path[path.length() - 1] == '/' && requested_path[0] == '/')
		path.erase(path.length() - 1);
	path += requested_path;
	return (path);
}

std::string	Response::checkIndex(const std::string& path)
{
	// * check all the available index by the location
	std::string	index_path = path;

	if (index_path[index_path.length() - 1] == '/')
		index_path.erase(index_path.length() - 1);
	index_path += "/index.html";
	if (PathVerifier::path_exists(index_path)) // ! maybe you check also if it's a file
		return (index_path);
	return ("");
}

void	Response::directoryListing(const std::string& path)
{
	DIR				*dir_ptr;
	struct dirent	*dir_entity;
	std::string		h1_header = "<h1>Director listing for ";

	h1_header += path;
	h1_header += "</h1>\n<hr>\n<ul>\n";
	// * list the content of the directory (path)
	if (!_location->get_autoindex())
		return ;
	for (size_t i = 0; i < h1_header.length(); ++i) // ! a lot of copying to the body make something to it..... !!!!!
		_body.push_back(h1_header[i]);
	dir_ptr = opendir(path.c_str());
	while ((dir_entity = readdir(dir_ptr)) != NULL)
	{
		// ** get name of teh entity and type
		
	}
	
}

void	Response::directoryServing(const std::string& path)
{
	std::string	index_path;

	// ** check index
	if (!(index_path = checkIndex(path)).empty())
	{
		// ! serve the index page..
		for (size_t i = 0; i < index_path.length(); ++i)
			_body.push_back(index_path[i]);
		return ;
	}
	directoryListing(path);
}

void	Response::requestedPathServe(const requestMessage& req)
{
	std::string	path;

	path = validateRootPath(req.getPath());
	if (!PathVerifier::path_exists(path))
	{
		// _body = _error_pages[NOT_FOUND];
		for (size_t i = 0; i < _error_pages[NOT_FOUND].length(); ++i)
			_body.push_back(_error_pages[NOT_FOUND][i]);
		return ;
	}
	if (PathVerifier::is_directory(path))
		directoryServing(path); // ** server index, or directory listing on case of autoindex
	
}

void	Response::buildResponse(const requestMessage& req, int status_code)
{
	// ** checking of status code, if is it not ok, respond with the associated page error
	checkErrorCode(status_code);
	// ! if the body empty so there is no error, and check the requested URI
	if (!_error_body)
		requestedPathServe(req);


	// ! header construction 
}


// *************** END *****************