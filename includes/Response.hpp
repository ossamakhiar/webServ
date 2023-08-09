/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:13:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/09 22:25:35 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "helpers.hpp"
#include "requestMessage.hpp"

class Response
{
private:
	std::map<int, std::string>	_error_pages;


	void	checkErrorCode(int status_code);

public:
	Response();
	~Response();
	Response(const Response&);
	Response&	operator=(const Response&);

	void	buildResponse(const requestMessage&, int status_code = OK);
	
};


#endif
