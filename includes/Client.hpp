/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:42:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/31 11:09:32 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sys/socket.h>
#include <netdb.h>

class Client
{
private:
	int					client_socket;
	struct sockaddr_in	client_addr;

public:
	Client();
	Client(int fd, struct sockaddr_in addr);
	~Client();
	Client(const Client& other);
	Client& operator=(const Client& rhs);
};

#endif