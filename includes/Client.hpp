/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:42:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/31 21:26:51 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "virtualServer.hpp"
#include <sys/socket.h>
#include <netdb.h>

class Client
{
private:
	int					client_socket;
	struct sockaddr_in	client_addr;

	virtualServer		*_vs;// ** vServer That client communicate to

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
};

#endif