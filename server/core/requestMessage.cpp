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

requestMessage::requestMessage() : _presistent_con(true), _content_len(0)
{

}

requestMessage::requestMessage(const requestMessage& other)
{
	_presistent_con = other._presistent_con;
	_content_len = other._content_len;
	(void)other;
}

requestMessage& requestMessage::operator=(const requestMessage& rhs)
{
	(void)rhs;
	return (*this);
}

requestMessage::~requestMessage()
{
	
}

void	requestMessage::fillRequestMessage(const std::string& req)
{
	this->_req_message = req;
	// for (size_t i = 0; i < _req_message.length(); i++)
	// {
		
	// }
}
