/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:57:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 20:48:06 by okhiar           ###   ########.fr       */
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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "Client.hpp"
#include "configParser.hpp"
#include "virtualServer.hpp"

# define BACKLOG	10

// ? fcntl(sockfd, F_SETFL, O_NONBLOCK);


class serverManager
{
private:
	std::vector<virtualServer>	_virtual_servers;
	std::map<int, std::vector<virtualServer*> >	_vs_endpoint;

	std::map<int, Client*>	_clients;
	int	highest_fd; // * the highest fd in fd_set

	fd_set read_fds, write_fds;
	fd_set pre_read_fds, pre_write_fds;

	// ** private member function
	void	serveClients(int);

	void	dropClient(int fd);
	bool	isPassiveSocket(int);
	void	newClient(int, struct sockaddr_in, virtualServer *);
	//void	readClientRequest(int); // ** read the client request to be processed later
	void	acceptNewConnection(); // ** accept new iteraction between client
	void	serveClients(void);
	//void	handleSocketEvents(); // ** Handle socket events based on whether they are passive or active

	int		createSocket(const char *, const char *);
	void	createPassiveSockets();

public:
	serverManager();
	~serverManager();
	serverManager(const serverManager&);
	serverManager& operator=(const serverManager&);

	void	IOSynchronous(void);

};

#endif