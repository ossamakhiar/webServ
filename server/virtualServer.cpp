/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:52:23 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/25 16:03:32 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "virtualServer.hpp"

virtualServer::virtualServer() : max_client_body_size(0)
{

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
	root = rhs.root;
	endpoint = rhs.endpoint;
	locations = rhs.locations;
	server_name = rhs.server_name;
	error_pages = rhs.error_pages;
	max_client_body_size = rhs.max_client_body_size;
	return (*this);
}

// TODO :: seters
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
	int							port;
	std::vector<std::string>	tokens;

	tokens = Helpers::split(listen, ":");
	if (tokens.size() != 2)
		throw std::runtime_error("bad listen directive");
	endpoint.first = tokens[0];
	port = Helpers::safeAtoi(tokens[1]);
	if (port < 0 || port > (1 << 16) - 1)
		throw std::runtime_error("bad listen port number");
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