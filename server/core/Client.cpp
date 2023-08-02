/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:44:06 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 14:34:58 by okhiar           ###   ########.fr       */
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

int				Client::getState(void) const
{
	return (client_state);
}

void	Client::readRequest()
{
	int			bytes;
	char		buffer[1024] = {0};
	size_t		header_end;
	std::string	req_msg;

	while (1)
	{
		bytes = read(client_socket, buffer, 1024);
		if (bytes == 0)
		{
			client_state = DISCONNECTED;
			return ;
		}
		buffer[bytes] = 0;
		req_msg.append(buffer, bytes);
		header_end = req_msg.find("\r\n\r\n");
		if (header_end != std::string::npos)
		{
			req_msg = req_msg.substr(0, header_end);
			break ;
		}
	}
	std::cout << "\e[1;34mRequest:\e[0m\n" << req_msg << std::endl;
	this->_request.fillRequestMessage(req_msg);
}

void	Client::makeResponse(void)
{
	write(client_socket, "HTTP/1.1 200 OK\n", 16);
	write(client_socket, "Server: oussama khiar\n\n", 23);
	write(client_socket, "<h1 style=\"color: green;\">Hello World</h1>\n", 41);
	std::cout << "+++++++ response sent ++++++++\n";
}