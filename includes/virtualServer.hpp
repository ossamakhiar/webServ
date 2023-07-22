/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 14:43:13 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/22 21:27:36 by okhiar           ###   ########.fr       */
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
	std::string											server_name;
	std::string											root;

public:
	virtualServer();
	~virtualServer();
	virtualServer(const virtualServer& other);
	virtualServer&	operator=(const virtualServer& rhs);

};

#endif