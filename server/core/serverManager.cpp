/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 13:03:00 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 22:25:24 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverManager.hpp"

serverManager::serverManager() : highest_fd(0)
{
	configParser	parser("./config/test.conf");

	_virtual_servers = parser.parseConfiguration();
	// std::cout << _virtual_servers.size() << std::endl;
	// for (std::vector<virtualServer>::iterator it = _virtual_servers.begin(); it != _virtual_servers.end(); ++it)
	// 	std::cout << *it << std::endl;
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
	highest_fd = socket_fd; // ** The Highest fd for select syscall
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
	int	socket_fd;

	for (std::vector<virtualServer>::iterator it = _virtual_servers.begin(); it != _virtual_servers.end(); ++it)
	{
		// ** map the vServer to the same socket that is listen in the same endpoint
		if (pre_listen.count((*it).getPort()))
		{
			_vs_endpoint[pre_listen[(*it).getPort()]].push_back(&(*it));
			continue ;
		}
		socket_fd = createSocket((*it).getEndpoint().first.c_str(), (*it).getPort().c_str());
		_vs_endpoint[socket_fd].push_back(&(*it));
		pre_listen[(*it).getPort()] = socket_fd;
	}
}

void	serverManager::newClient(int fd, struct sockaddr_in addr, virtualServer *vs)
{
	Client *client;

	client = new Client(fd, addr);
	// ** check the vServer that communicate to & set it to the client
	client->setVS(vs);
	_clients[fd] = client;
}

void	serverManager::dropClient(int fd)
{
	Client	*c;

	if (fd == highest_fd)
		highest_fd--;
	close(fd);
	c = _clients[fd];
	delete c;
}

bool	serverManager::isPassiveSocket(int fd)
{
	return (_vs_endpoint.count(fd) > 0);
}

void	serverManager::acceptNewConnection(void)
{
	int					newsock;
	struct sockaddr_in	addr;
	socklen_t 			len = sizeof(struct sockaddr_in);

	for (std::map<int, std::vector<virtualServer*> >::iterator it = _vs_endpoint.begin(); it != _vs_endpoint.end(); ++it)
	{
		// * check if this passive socket ready to read
		if (!FD_ISSET(it->first, &pre_read_fds))
			continue ;
		newsock = accept(it->first, (struct sockaddr*)&addr, &len);
		// ! fctnl()
		if (newsock >= 1024) // * the highest fd number
		{
			// ! temporary printing
			std::cout << "\e[1;31mconnection refused\e[0m -- the sockets set full" << std::endl;
			close(newsock);
			return ;
		}
		FD_SET(newsock, &read_fds);
		highest_fd = (newsock > highest_fd ? newsock : highest_fd);
		std::cout << "\e[1;32mnew connection from: " << inet_ntoa(addr.sin_addr) << "::" << ntohs(addr.sin_port) << "\e[0m\n";
		newClient(newsock, addr, it->second.front());
	}
}

void	serverManager::serveClients(void)
{
	int	fd;
	std::vector<int>	keys_erase;

	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		fd = it->first;
		if (FD_ISSET(fd, &pre_read_fds))
		{
			it->second->readRequest();
			if (it->second->getState() == DISCONNECTED)
			{
				dropClient(fd);// ** BYE BYE
				FD_CLR(fd, &read_fds);
				keys_erase.push_back(fd);
				continue ;
			}
			FD_SET(fd, &write_fds);
			FD_CLR(fd, &read_fds);
		}
		else if (FD_ISSET(fd, &pre_write_fds))
		{
			it->second->makeResponse();
			// TODO :: check if it's a presistent connection & handle it
			dropClient(fd);
			FD_CLR(fd, &write_fds);
			keys_erase.push_back(fd);
		}
	}
	for (std::vector<int>::iterator it = keys_erase.begin(); it != keys_erase.end(); ++it)
		_clients.erase(*it);
}

void	serverManager::IOSynchronous(void)
{
	while (1)
	{
		FD_COPY(&read_fds, &pre_read_fds);
		FD_COPY(&write_fds, &pre_write_fds);
		if (select(highest_fd + 1, &pre_read_fds, &pre_write_fds, 0, 0) < 0)
			return ; // ! handle this
		// ** check if there a passive socket ready to read from 
		// ** then := create an active socket that 
		acceptNewConnection();
	
		// ** Serve the conected client through an active socket...
		serveClients();
	}
}
