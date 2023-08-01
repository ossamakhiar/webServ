/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:44:06 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/31 21:26:06 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{

}

Client::Client(int fd, struct sockaddr_in addr) : _vs(NULL)
{
	this->client_socket = fd;
	this->client_addr = addr;
}

Client::~Client()
{

}

Client::Client(const Client& other)
{
	*this = other;
}

Client& Client::operator=(const Client& rhs)
{
	if (this == &rhs)
		return (*this);
	this->client_socket = rhs.client_socket;
	this->client_addr = rhs.client_addr;
	return (*this);
}

void	Client::setVS(virtualServer *vs)
{
	this->_vs = vs;
}

virtualServer	*Client::getVS(void) const
{
	return (_vs);
}