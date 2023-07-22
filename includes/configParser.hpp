/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:36:27 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/22 21:26:57 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <fstream>
#include <iostream>
#include "configSyntax.hpp"
#include "virtualServer.hpp"

class configParser
{
private:
	std::ifstream				config_file;
	std::vector<virtualServer>	virtualServers;

	// ** private member functions
	

	configParser(const configParser& other);
	configParser&	operator=(const configParser& rhs);
public:
	// ** Contructors
	configParser();
	configParser(const std::string& file_name);
	~configParser();

	std::vector<virtualServer>	parseConfiguration();
};

#endif