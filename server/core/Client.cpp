/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:44:06 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/03 14:50:16 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : client_state(READING_REQUEST)
{

}

Client::Client(int fd, struct sockaddr_in addr) : _vs(NULL)
{
	client_state = READING_REQUEST;
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
	client_state = rhs.client_state;
	this->client_socket = rhs.client_socket;
	this->client_addr = rhs.client_addr;
	return (*this);
}

// TODO :: Setters
void	Client::setVS(virtualServer *vs)
{
	this->_vs = vs;
}


// TODO :: Getters
virtualServer	*Client::getVS(void) const
{
	return (_vs);
}

int				Client::getState(void) const
{
	return (client_state);
}


// TODO :: client handler
void	Client::readRequest()
{

	try {
		this->_request.requestHandling(client_socket);
		// * this state of the request handler will set when the parsing phase ends
		if (_request.getReqState() == DONE_REQ)
		{
			client_state = BUILD_RESPONSE;
			std::cout << _request << std::endl;
		}
	} catch (e_status_code code) {
		_status_code = code;
		std::cout << "ERROR THROWED: " << code << std::endl;
		if (code == ZERO_READED)
			client_state = DISCONNECTED;
	}
}

void	Client::makeResponse(void)
{
	write(client_socket, "HTTP/1.1 200 OK\n", 16);
	write(client_socket, "Server: oussama khiar\n\n", 23);
	write(client_socket, "<h1 style=\"color: green;\">Hello World</h1>\n", 41);
	std::cout << "+++++++ response sent ++++++++" << std::endl;
}
