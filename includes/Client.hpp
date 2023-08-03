/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:42:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/03 14:28:41 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "virtualServer.hpp"
#include "requestMessage.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

enum e_client_states
{
	READING_REQUEST,
	BUILD_RESPONSE,
	DISCONNECTED
};

class Client
{
private:
	int					client_state;
	int					client_socket;
	struct sockaddr_in	client_addr;

	virtualServer		*_vs;// ** vServer That client communicate to

	requestMessage		_request;
	e_status_code		_status_code;

public:
	Client(int fd, struct sockaddr_in addr);
	Client();
	~Client();
	Client(const Client& other);
	Client& operator=(const Client& rhs);

	// TODO :: Setters
	void	setVS(virtualServer *vs);

	// TODO :: Getters
	virtualServer	*getVS(void) const;
	int				getState(void) const;

	void	readRequest(void);
	void	makeResponse(void);
};

#endif