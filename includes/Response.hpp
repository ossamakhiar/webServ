/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:13:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/11 17:55:29 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <dirent.h>
#include "helpers.hpp"
#include "PathVerifier.hpp"
// #include "fString"
#include "requestMessage.hpp"

enum e_responsing_states
{
	BODY_PRODUCING,
	CGI_WAITING,
	RESPONSE_SENDING
};

class Response
{
private:
	typedef void (Response::*StationsPtr)(void);
	typedef void (Response::*MethodHandler)(void);

	int	_client_socket;

	int	_responsing_state;
	std::map<int, StationsPtr>				_stations;
	std::map<std::string, MethodHandler>	_methods_handler;

	std::string	_html_start;
	std::string	_html_close;

	bool	_error_body;

	std::map<int, std::string>	_reason_phrase; // ** map a status code with it reason
	std::map<int, std::string>	_error_pages;

	std::string	_redirection_path;

	std::string	_headers;
	std::string	_body; // ? response body

	virtualServer	*_vs;
	locationBlock	*_location;

	int						_status_code;
	const requestMessage	*_req;
	std::string				_request_method;



	void	fillReasonPhrases(void);
	void	fillDefaultErrorPages(void);
	// std::vector<char>&	operator=(std::vector<char>& , const std::string&);

	std::string	checkIndex(const std::string&);
	void		directoryListing(const std::string&);
	void		directoryServing(const std::string&);

	std::string	validateRootPath(const std::string&);

	void	checkErrorCode(int);
	// void	requestedPathServe(const requestMessage&);
	void	fileServing(const std::string& path);

	void	responseHeader(void);
	void	respond(void);
	// * Body producers
	void	bodyProdcucers(void);

	// * methods handlers
	void	getHandler(void);

	void	setRequest(const requestMessage&, int);

public:
	Response();
	~Response();
	Response(const Response&);
	Response&	operator=(const Response&);

	// * setters
	void	setVS(virtualServer*);
	void	setLocation(locationBlock*);

	void	buildResponse(const requestMessage&, int client_socket, int status_code = OK);
	
};

#endif
