/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 13:03:00 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/31 18:55:32 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverManager.hpp"

serverManager::serverManager() : nfds(0)
{
	configParser	parser("./config/test.conf");

	_virtual_servers = parser.parseConfiguration();
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	// FD_ZERO(&pre_read_fds);
	// FD_ZERO(&pre_write_fds);
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
	nfds = socket_fd; // ** The Highest fd for select syscall
	// std::cout << "fd: " << socket_fd << std::endl;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(socket_fd, res->ai_addr, res->ai_addrlen) < 0)
		throw Helpers::exceptionError("socket binding failed!");
	if (listen(socket_fd, BACKLOG))
		throw Helpers::exceptionError("listening failed");
	freeaddrinfo(res);
	return (socket_fd);
}

// TODO :: Create the Passive Sockets that listen for incoming connections
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

void	serverManager::newClient(int fd, struct sockaddr_in addr)
{
	Client *client;

	client = new Client(fd, addr);
	_clients[fd] = client;
}

void	serverManager::dropClient(int fd)
{
	Client	*c;

	c = _clients[fd];
	_clients.erase(fd);
	delete c;
}

bool	serverManager::isPassiveSocket(int fd)
{
	return (_vs_endpoint.count(fd) > 0);
}

void	serverManager::acceptNewConnection(int fd)
{
	int					newsock;
	struct sockaddr_in	addr;
	socklen_t 			len = sizeof(struct sockaddr_in);

	newsock = accept(fd, (struct sockaddr*)&addr, &len);
	if (newsock >= 1024) // * the highest fd number
	{
		// ! temporary printing
		std::cout << "\e[1;31mconnection refused\e[0m -- the sockets set full" << std::endl;
		close(newsock);
		return ;
	}
	FD_SET(newsock, &read_fds);
	nfds = (newsock > nfds ? newsock : nfds);
	std::cout << "\e[1;32mnew connection from: " << inet_ntoa(addr.sin_addr) << "::" << ntohs(addr.sin_port) << "\e[0m\n";
	newClient(newsock, addr);
}

void	serverManager::readClientRequest(int fd)
{
	int		bytes;
	char	buffer[1024] = {0};

	bytes = read(fd, buffer, 1024);
	if (bytes == 0)
	{
		dropClient(fd);// ** BYE BYE
		close(fd);
		FD_CLR(fd, &read_fds);
		return ;
	}
	std::cout << buffer << std::endl;
	FD_SET(fd, &write_fds);
	FD_CLR(fd, &read_fds);
}

void	serverManager::handleSocketEvents(int fd)
{
	bool	is_passive;

	is_passive = isPassiveSocket(fd);
	// TODO :: check if fd exists on passive socket, then create an active socket
	if (is_passive)
	{
		acceptNewConnection(fd);
		return ;
	}
	readClientRequest(fd);
}

void	serverManager::serveClients(int fd)
{
	write(fd, "HTTP/1.1 200 OK\n", 16);
	write(fd, "Server: oussama khiar\n\n", 23);
	write(fd, "<h1 style=\"color: green;\">Hello World</h1>\n", 41);
	std::cout << "+++++++ request sent ++++++++\n";
	// TODO :: check if it's a presistent connection & handle it
	dropClient(fd);
	close(fd);
	FD_CLR(fd, &write_fds);
}

void	serverManager::IOSynchronous(void)
{
	fd_set pre_read_fds, pre_write_fds;

	while (1)
	{
		FD_COPY(&read_fds, &pre_read_fds);
		FD_COPY(&write_fds, &pre_write_fds);
		if (select(nfds + 1, &pre_read_fds, &pre_write_fds, 0, 0) < 0)
			return ; // ! handle this
		for (int i = 0; i <= nfds; ++i)
		{
			if (FD_ISSET(i, &pre_read_fds))
				handleSocketEvents(i);
			else if (FD_ISSET(i, &pre_write_fds))
				serveClients(i);
		}
	}
}
