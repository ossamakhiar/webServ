/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:44:06 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/12 16:15:44 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, struct sockaddr_in addr, std::vector<virtualServer*>	&ends_vs) : _vs_endpoint(ends_vs), \
	_vs(NULL),
	_location(NULL),
	_request(_vs_endpoint, _vs, _location)
{
	_status_code = OK;
	client_state = READING_REQUEST;
	this->client_socket = fd;
	this->client_addr = addr;
}

Client::~Client()
{

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

void	Client::responseImportantSettings(void)
{
	_response.setVS(_vs);
	_response.setLocation(_location);
	_response.setRequest(&_request);
	_response.setStatusCode(_status_code);
	_response.setRequestMethod(_request.getMethod());
}

// TODO :: client handler
void	Client::readRequest()
{

	try {
		this->_request.requestHandling(client_socket);
		// * this state of the request handler will set when the parsing phase and body reading ends
		if (_request.getReqState() == DONE_REQ)
		{
			client_state = BUILD_RESPONSE;
			std::cout << _request << std::endl;
		}
	} catch (e_status_code code) {
		// ! you should close the file descriptor of the post body
		_status_code = code;
		client_state = BUILD_RESPONSE;
		std::cout << "ERROR THROWED: " << code << std::endl;
		if (code == ZERO_READED)
			client_state = DISCONNECTED;
	}

	// ! set important Reponse fileds
	if (client_state == BUILD_RESPONSE)
		responseImportantSettings();
}

void	Client::makeResponse(void)
{
	std::cout << "send request by meeeee\n";

	try{
		this->_response.buildResponse(client_socket);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	if (_response.getResponseState() == RESPONSE_DONE)
		client_state = CLIENT_DONE;


	// ******
	// std::cout << "\e[1;31mLocation: \e[0m" << _request.getURI() << std::endl;
	// if (_location)
	// 	std::cout << *_location << std::endl;
	// if (_vs)
	// 	std::cout << *_vs << std::endl;
}
