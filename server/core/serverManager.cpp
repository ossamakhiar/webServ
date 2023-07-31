/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 13:03:00 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/30 22:32:17 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverManager.hpp"

serverManager::serverManager() : nfds(0)
{
	configParser	parser("./config/test.conf");

	_virtual_servers = parser.parseConfiguration();
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&pre_read_fds);
	FD_ZERO(&pre_write_fds);
	createPassiveSockets();
	for (std::map<int, std::vector<virtualServer*> >::iterator it = _vs_endpoint.begin(); it != _vs_endpoint.end(); ++it)
	{
		std::cout << it->first << std::endl;
		std::cout << it->second.size() << std::endl;
		for (std::vector<virtualServer*>::iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it)
		{
			std::cout << "\t";
			std::cout << (*vec_it)->getPort() << ":" << (*vec_it)->getEndpoint().first << std::endl;
		}
	}
	for (std::map<int, std::vector<virtualServer*> >::iterator it = _vs_endpoint.begin(); it != _vs_endpoint.end(); ++it)
		FD_SET(it->first, &read_fds);
	// nfds = _vs_endpoint.size();
}

serverManager::~serverManager()
{

}

serverManager::serverManager(const serverManager& other)
{
	// TODO :: Copying
	(void)other;
}

serverManager& serverManager::operator=(const serverManager& rhs)
{
	// TODO :: Assigning
	(void)rhs;
	return (*this);
}

// ** Socket creation ...
int	serverManager::createSocket(const char *hostname, const char *port)
{
	struct	addrinfo	hints, *res;
	int	socket_fd, yes = 1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(hostname, port, &hints, &res);
	if ((socket_fd = socket(res->ai_family, res->ai_socktype, 0)) < 0)
		throw Helpers::exceptionError("socket creation failed!");
	nfds = socket_fd;
	// std::cout << "fd: " << socket_fd << std::endl;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(socket_fd, res->ai_addr, res->ai_addrlen) < 0)
		throw Helpers::exceptionError("socket binding failed!");
	if (listen(socket_fd, BACKLOG))
		throw Helpers::exceptionError("listening failed");
	freeaddrinfo(res);
	return (socket_fd);
}

// ** Create the Passive Socket that listen for incoming connections
void	serverManager::createPassiveSockets(void)
{
	std::map<std::string, int>				pre_listen;
	std::map<std::string, int>::iterator	itlow;
	int	socket_fd;

	for (std::vector<virtualServer>::iterator it = _virtual_servers.begin(); it != _virtual_servers.end(); ++it)
	{
		// ** map the vServer to the same socket that is listen in the same endpoint
		if ((itlow = pre_listen.lower_bound((*it).getPort()))->first == (*it).getPort())
		{
			_vs_endpoint[itlow->second].push_back(&(*it));
			continue ;
		}
		socket_fd = createSocket((*it).getEndpoint().first.c_str(), (*it).getPort().c_str());
		_vs_endpoint[socket_fd].push_back(&(*it));
		pre_listen[(*it).getPort()] = socket_fd;
	}
}

void	serverManager::IOSynchronous(void)
{
	int	newsock;
	struct sockaddr_in	addr;
	socklen_t len = sizeof(struct sockaddr_in);
	char buffer[1024];
	int	bytes;

	while (1)
	{
		FD_COPY(&read_fds, &pre_read_fds);
		FD_COPY(&write_fds, &pre_write_fds);
		if (select(nfds + 1, &pre_read_fds, &pre_write_fds, 0, 0) < 0)
			return ; // ! handle this
		// newConnection();
		for (int i = 0; i <= nfds; ++i)
		{
			if (FD_ISSET(i, &pre_read_fds))
			{
				try {
					_vs_endpoint.at(i);
					newsock = accept(i, (struct sockaddr*)&addr, &len);
					FD_SET(newsock, &read_fds);
					nfds = (newsock > nfds ? newsock : nfds);
				} catch (std::exception& e) {
					bytes = read(i, buffer, 1024);
					if (bytes == 0)
					{
						close(i); // ** BYE BYE
						FD_CLR(i, &read_fds);
						continue ;
					}
					std::cout << buffer << std::endl;
					FD_SET(i, &write_fds);
					FD_CLR(i, &read_fds);
				}
			}
			else if (FD_ISSET(i, &pre_write_fds))
			{
				write(i, "HTTP/1.1 200 OK\n", 16);
				write(i, "Server: oussama khiar\n\n", 23);
				write(i, "<h1 style=\"color: red;\">Hello World <br>Salah sben-ela</br></h1>\n", 65);
				std::cout << "+++++++ request sent ++++++++\n";
				close(i);
				FD_CLR(i, &write_fds);
			}
		}
		
	}
}
