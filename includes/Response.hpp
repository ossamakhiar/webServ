/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:13:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/12 15:56:56 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <dirent.h>
#include <sys/wait.h>
#include "helpers.hpp"
#include "ftString.hpp"
#include "PathVerifier.hpp"
// #include "fString"
#include "requestMessage.hpp"

#define READ_BUFFER 8192

enum e_responsing_states
{
	BODY_PRODUCING,
	CGI_WAITING,
	RESPONSE_SENDING,
	RESPONSE_DONE
};

enum e_stored_types
{
	RAM_BUFFER,
	EXTERNAL_STORAGE
};

class Response
{
private:
	typedef void (Response::*StationsPtr)(void);
	typedef void (Response::*MethodHandler)(void);
	std::string	_html_start;
	std::string	_html_close;

	int	_client_socket;
	int	_content_len;


	int										_responsing_state;
	std::map<int, StationsPtr>				_stations;
	std::map<std::string, MethodHandler>	_methods_handler;


	std::map<int, std::string>	_reason_phrase; // ** map a status code with it reason
	std::map<int, std::string>	_error_pages;

	std::string					_redirection_path;
	std::string					_resource_physical_path;
	std::string					_index_full_path;

	int							_body_fd;
	std::string					_headers;
	std::string					_body; // ? response body, ftString, a raw data buffer it cares about the it length only

	virtualServer				*_vs;
	locationBlock				*_location;

	int							_status_code;
	const requestMessage		*_req;
	std::string					_request_method;

	int							_cgi_pid;
	bool						_cgi_exists;
	std::string					_cgi_ext;
	std::string					_cgi_outfile;
	std::string					_cgi_headers;

	int							_stored_type;




	std::string					checkIndex(const std::string&);
	void						directoryListing(const std::string&);
	void						directoryServing(const std::string&);
	void						DirectoryRequestedChecking(const std::string&);


	void						checkErrorCode(int);

	void						body_buffer_sending(void);
	void						file_chunks_sending(void);
	void						reponseSending(void);
	void						responseHeader(void);
	void						respond(void);

	std::vector<std::string>	cgi_env_setting(void);
	bool						checkCgiExistence(void);
	void						redirect_cgi_input(void);
	void						redirect_cgi_output();
	void						cgi_handler(void);

	void						cgiWaiting(void);
	void						responsePrepering(void);

	void	fileServing(void);

	// * methods handlers
	void	getHandler(void);
	void	postHandler(void);
	void	deleteHandler(void);

	// * HELPERS
	void		fillReasonPhrases(void);
	void		fillDefaultErrorPages(void);
	std::string	validateRootPath(const std::string&);
	void		cgiHeaderExtracting(void);
	void		setContentLength(void);
	std::string	getScriptName() const;
	void		setRequest(const requestMessage&, int);
	bool		_isIndex(void) const;

public:
	Response();
	~Response();
	Response(const Response&);
	Response&	operator=(const Response&);

	// * getters
	int		getResponseState(void) const;

	// * setters
	void	setVS(virtualServer*);
	void	setLocation(locationBlock*);
	void	setRequest(requestMessage *);
	void	setStatusCode(int);
	void	setRequestMethod(const std::string&);

	void	buildResponse(int client_socket);
	
};

#endif
