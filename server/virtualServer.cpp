/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:52:23 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/23 21:33:07 by okhiar           ###   ########.fr       */
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
	endpoint = rhs.endpoint;
	locations = rhs.locations;
	server_name = rhs.server_name;
	error_pages = rhs.error_pages;
	max_client_body_size = rhs.max_client_body_size;
	// root = rhs.root;
	return (*this);
}

// ? .......

void	virtualServer::setServerName(const std::string& name)
{
	this->server_name = name;
}

void	virtualServer::setEndpoint(const std::string& listen)
{
	endpoint.first = listen;
	endpoint.second = 80;
}

void	virtualServer::setErrorPage(const std::string& err)
{
	error_pages.push_back(std::pair<std::string, std::string>("404", err));
}

void	virtualServer::setMaxBodySize(const std::string& body_s)
{
	max_client_body_size = std::stoi(body_s);
}

std::ostream& operator<<(std::ostream& os, const virtualServer& vs)
{
	os << "server_name: " << vs.server_name << std::endl;
	os << "endpoint: " << vs.endpoint.first << "[" << vs.endpoint.second << "]" << std::endl;
	os << "MaxBodySize: " << vs.max_client_body_size << std::endl;
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = vs.error_pages.begin(); it != vs.error_pages.end(); ++it)
		os << "error_page: " << it->first << " ==> " << it->second << std::endl;
	return (os);
}