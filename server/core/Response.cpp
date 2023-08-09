/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:06:53 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/09 22:28:31 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	
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

// ** RESPONSING....

void	Response::checkErrorCode(int status_code)
{
	(void)status_code;
}

void	Response::buildResponse(const requestMessage& req, int status_code)
{
	// ** checking of status code, if is it not ok, respond with the associated page error
	checkErrorCode(status_code);
}