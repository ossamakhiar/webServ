/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:52:23 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 23:06:03 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "virtualServer.hpp"

virtualServer::virtualServer() : max_client_body_size(500000)
{
	endpoint.first = "";
	endpoint.second = -1;
	server_name = "";
	root = "";
	port = "";
}

virtualServer::~virtualServer()
{

}

virtualServer::virtualServer(const virtualServer& other)
{
	*this = other;
}

virtualServer&	virtualServer::operator=(const virtualServer& rhs)
{
	if (this == &rhs)
		return (*this);
	port = rhs.port;
	root = rhs.root;
	endpoint = rhs.endpoint;
	locations = rhs.locations;
	server_name = rhs.server_name;
	error_pages = rhs.error_pages;
	max_client_body_size = rhs.max_client_body_size;
	return (*this);
}

// TODO :: Getters
locationBlock&	virtualServer::getLocations(const std::string& key)
{
	return (locations[key]);
}

const std::string&	virtualServer::getPort() const
{
	return (port);
}

const std::pair<std::string, int>& virtualServer::getEndpoint() const
{
	return (endpoint);
}

const std::string&	virtualServer::getRootDir(void) const
{
	return (root);
}

// TODO :: Setters
void	virtualServer::setServerName(const std::string& name)
{
	this->server_name = name;
}

void	virtualServer::setRootDir(const std::string& r)
{
	this->root = Helpers::trim(r);
}

void	virtualServer::setEndpoint(const std::string& listen)
{
	int							port, ret;
	struct addrinfo				hints, *res;
	std::vector<std::string>	tokens;

	tokens = Helpers::split(Helpers::trim(listen), ":");
	if (tokens.size() != 2)
		throw std::runtime_error("bad listen directive");
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	ret = getaddrinfo(tokens[0].c_str(), tokens[1].c_str(), &hints, &res); // ! that a create a unix domain socket "why ?""
	if (ret)
		throw Helpers::exceptionError("bad endpoint input");
	freeaddrinfo(res);
	this->port = tokens[1];
	endpoint.first = tokens[0];
	port = Helpers::safeAtoi(tokens[1]);
	endpoint.second = port;
}

void	virtualServer::setErrorPage(const std::string& err)
{
	std::vector<std::string>	tokens;

	tokens = Helpers::split(err, " \t");
	error_pages.push_back(std::pair<int, std::string>(Helpers::safeAtoi(tokens[0]), tokens[1])); // ** maybe map container better
}

void	virtualServer::setMaxBodySize(const std::string& body_s)
{
	max_client_body_size = Helpers::safeAtoi(body_s);
}


// void	virtualServer::setDefaultRoot(void) // TODO :: set default root to the location, which taken from the server root
// {
// 	for (std::map<std::string, locationBlock>::iterator it = locations.begin(); it != locations.end(); ++it)
// 		if (it->second.getRoot().empty())
// 			it->second.setRoot(root);
// }

// *******************END****************

std::ostream& operator<<(std::ostream& os, const virtualServer& vs)
{
	os << "root: " << vs.root << std::endl;
	os << "server_name: " << vs.server_name << std::endl;
	os << "endpoint: " << vs.endpoint.first << "[" << vs.endpoint.second << "]" << std::endl;
	os << "MaxBodySize: " << vs.max_client_body_size << std::endl;
	for (std::vector<std::pair<int, std::string> >::const_iterator it = vs.error_pages.begin(); it != vs.error_pages.end(); ++it)
		os << "error_page: [" << it->first << "] ==> " << it->second << std::endl;
	std::cout << "Locations: \n";
	for (std::map<std::string, locationBlock>::const_iterator it = vs.locations.begin(); it != vs.locations.end(); ++it)
		std::cout << "\t" << it->first << ": \n" << it->second << std::endl;
	return (os);
}