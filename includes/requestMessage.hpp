/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:37:42 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 14:12:16 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTMESSAGE_HPP
# define REQUESTMESSAGE_HPP

#include "helpers.hpp"

class requestMessage
{
private:
	std::string	_req_message; // ** store the request message

	bool		_presistent_con;
	int			_content_len; // ! mandatory in case request has a body message
	std::string	_hostname; // ! make it mandatory :)
	std::string	_method;
	std::string	_path;

public:
	requestMessage();
	~requestMessage();
	requestMessage(const requestMessage&);
	requestMessage& operator=(const requestMessage&);

	void	fillRequestMessage(const std::string& req);


};

#endif
