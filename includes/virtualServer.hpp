/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 14:43:13 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/23 21:29:36 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map> // ** map host with port listening on

class	virtualServer
{
private:
	std::pair<std::string, int>							endpoint;// ** map host with port listening on
	std::map<std::string, std::vector<std::string> >	locations; // ** map every location with its directives
	std::vector<std::pair<std::string, std::string> >	error_pages;
	std::string											server_name;

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

	// const std::string& getServerName(void) const{
	// 	return server_name;
	// }

	// ! remove this
	friend std::ostream& operator<<(std::ostream& os, const virtualServer& vs);
};

std::ostream& operator<<(std::ostream& os, const virtualServer& vs);

#endif