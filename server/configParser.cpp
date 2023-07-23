/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:47:16 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/23 21:07:19 by okhiar           ###   ########.fr       */
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
	// ** Location Directives DB
	// locationDirectives.insert(std::pair<std::string, int>("allowed_methods", 3));
	// locationDirectives.insert(std::pair<std::string, int>("directory_listing", 1));
	// locationDirectives.insert(std::pair<std::string, int>("root", 1));
	// locationDirectives.insert(std::pair<std::string, int>("cgi", 2));
	// locationDirectives.insert(std::pair<std::string, int>("upload_post", 1));

	// ** Server Directives DB
	serverDirectives.insert(std::pair<std::string, funcSeter>("listen", &virtualServer::setEndpoint));
	serverDirectives.insert(std::pair<std::string, funcSeter>("server_name", &virtualServer::setServerName));
	// serverDirectives.insert(std::pair<std::string, funcSeter>("max_client_body_size", 1));
	serverDirectives.insert(std::pair<std::string, funcSeter>("error_page", &virtualServer::setErrorPage));
}

configParser::~configParser()
{
	config_file.close();
}

size_t	configParser::parseLocationBlock(virtualServer& vs, const std::string& buffer)
{
	size_t	i;

	i = 0;
	// TODO
	for ( ; buffer[i] != '}'; i++)
		;
	(void)vs;
	return (i);
}

size_t	configParser::parseDirectives(virtualServer& vs, const std::string& buff, int& blocks)
{
	size_t		pos;
	size_t		lineLength;
	std::string	directive;

	if ((pos = buff.find_first_of(" \t")) == std::string::npos)
		throw std::runtime_error("error");
	directive = buff.substr(0, pos);
	if (directive == "location")
	{
		blocks++;
		return (parseLocationBlock(vs, buff));
	}
	lineLength = Helpers::sepDistance(buff, '\n');
	(vs.*(serverDirectives[directive]))(buff.substr(pos, lineLength - pos));
	return (lineLength);
}

virtualServer	configParser::parseServerBlock(std::string& buffer)
{
	virtualServer	vs;
	size_t			i;
	int				blocks;

	blocks = 1;
	i = Helpers::sepDistance(buffer, '{') + 1;
	while (blocks && buffer[i])
	{
		if (buffer[i] == '\n' && ++i)
			continue ;
		if (buffer[i] == '{' || buffer[i] == '}')
		{
			blocks += (1 - 2 * (buffer[i++] == '}'));
			continue ;
		}
		buffer.erase(0, i);
		i = parseDirectives(vs, buffer, blocks);
		buffer.erase(0, i);
		i = 0;
	}
	buffer.erase(0, i);
	return (vs);
}

std::vector<virtualServer>	configParser::parseConfiguration(void)
{
	std::vector<virtualServer>	virtualServers;
	configSyntax				syntaxChecker;
	std::string					buffer;
	size_t skippedLines;

	try{
		buffer = syntaxChecker.syntaxEvaluation(config_file);
	} catch (std::exception& e) {
		std::cout << e.what();
		return (virtualServers);
	}
	while (!buffer.empty())
	{
		virtualServers.push_back(parseServerBlock(buffer));
		skippedLines = buffer.find_first_not_of("\n");
		// ** skipped new line that stay in the string to avoid extra call
		buffer.erase(0, skippedLines);
	}
	std::cout << virtualServers.size() << std::endl;
	for (std::vector<virtualServer>::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it)
		std::cout << *it << std::endl;
	return (virtualServers);
}