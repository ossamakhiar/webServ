/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:57:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/30 22:29:27 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "configParser.hpp"
#include "virtualServer.hpp"

# define BACKLOG	10

// ? fcntl(sockfd, F_SETFL, O_NONBLOCK);


class serverManager
{
private:
	std::vector<virtualServer>	_virtual_servers;
	std::map<int, std::vector<virtualServer*> >	_vs_endpoint;

	int	nfds;

	fd_set read_fds, write_fds;
	fd_set pre_read_fds, pre_write_fds;

	// ** private member function
	int		createSocket(const char *hostname, const char *port);
	void	createPassiveSockets();

public:
	serverManager();
	~serverManager();
	serverManager(const serverManager&);
	serverManager& operator=(const serverManager&);

	void	IOSynchronous(void);

};

#endif