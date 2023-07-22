/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:47:16 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/22 21:27:54 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParser.hpp"

configParser::configParser()
{

}

configParser::configParser(const std::string& file_name)
{
	config_file.open(file_name.c_str(), std::ios::in);
	if (!config_file.is_open())
		throw std::runtime_error("bad input file");
}

configParser::~configParser()
{
	config_file.close();
}

std::vector<virtualServer>	configParser::parseConfiguration(void)
{
	std::vector<virtualServer>	virtualServers;
	configSyntax				syntaxChecker;
	std::string					buffer;

	try{
		buffer = syntaxChecker.syntaxEvaluation(config_file);
		std::cout << buffer;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return (virtualServers);
}