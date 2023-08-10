/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:13:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/10 17:01:49 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <dirent.h>
#include "helpers.hpp"
#include "PathVerifier.hpp"
// #include "fString"
#include "requestMessage.hpp"

class Response
{
private:
	bool	_error_body;

	std::map<int, std::string>	_reason_phrase; // ** map a status code with it reason
	std::map<int, std::string>	_error_pages;

	std::vector<char>	_body;

	virtualServer	*_vs;
	locationBlock	*_location;


	void fillDefaultErrorPages(void);
	// std::vector<char>&	operator=(std::vector<char>& , const std::string&);

	std::string	checkIndex(const std::string&);
	void		directoryListing(const std::string&);
	void		directoryServing(const std::string&);

	std::string	validateRootPath(const std::string&);

	void	checkErrorCode(int);
	void	requestedPathServe(const requestMessage&);

public:
	Response();
	~Response();
	Response(const Response&);
	Response&	operator=(const Response&);

	// * setters
	void	setVS(virtualServer*);
	void	setLocation(locationBlock*);

	void	buildResponse(const requestMessage&, int status_code = OK);
	
};

#endif
