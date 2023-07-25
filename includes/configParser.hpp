/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:36:27 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/25 15:43:56 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <fstream>
#include <iostream>
#include "configSyntax.hpp"
#include "virtualServer.hpp"

typedef void (virtualServer::*funcSeter)(const std::string&);
typedef void (locationBlock::*ldSeter)(const std::string&);

class configParser
{
private:
	std::ifstream						config_file;
	std::map<std::string, ldSeter>		locationDirectives;
	std::map<std::string, funcSeter>	serverDirectives;
	// std::vector<virtualServer>			virtualServers;

	// ** private member functions
	std::pair<std::string, std::string>	locationSettings(const std::string& buff, size_t& i);

	size_t			parseLocationBlock(virtualServer&, const std::string&);
	size_t			parseDirectives(virtualServer&, const std::string&, int&);
	virtualServer	parseServerBlock(std::string& buffer);

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