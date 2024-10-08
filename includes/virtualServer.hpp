/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 14:43:13 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/08 17:17:01 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <netdb.h>
#include <string.h>
#include "locationBlock.hpp"


class	virtualServer
{
private:
	std::pair<std::string, int>					endpoint;// ** map host with port listening on
	std::map<std::string, locationBlock>		locations; // ** map every location with its directives
	std::vector<std::pair<int, std::string> >	error_pages;
	std::string									server_name;
	std::string									root;
	std::string									port; // ** Store the port in string to use it in getaddrinfo

	int	max_client_body_size;

public:
	virtualServer();
	~virtualServer();
	virtualServer(const virtualServer& other);
	virtualServer&	operator=(const virtualServer& rhs);

	void	setServerName(const std::string&);
	void	setEndpoint(const std::string&);
	void	setErrorPage(const std::string&);
	void	setMaxBodySize(const std::string&);
	void	setRootDir(const std::string&);

	// TODO :: Getters
	std::map<std::string, locationBlock>& getLocations(void)
	{
		return (locations);
	}


	locationBlock&		getLocations(const std::string& key);
	const std::string&	getPort() const;
	const std::pair<std::string, int>& getEndpoint() const;
	const std::string&	getRootDir(void) const;
	int					getMaxBodySize(void) const;

	// ! this function set to the every location that don't have a root dir to the server root
	//void	setDefaultRoot(void);

	// ! remove this
	friend std::ostream& operator<<(std::ostream& os, const virtualServer& vs);
};

std::ostream& operator<<(std::ostream& os, const virtualServer& vs);

#endif