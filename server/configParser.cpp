/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:47:16 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/30 19:09:59 by okhiar           ###   ########.fr       */
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
	locationDirectives.insert(std::pair<std::string, ldSeter>("root", &locationBlock::setRoot));
	locationDirectives.insert(std::pair<std::string, ldSeter>("allowed_methods", &locationBlock::setAllowedMethods));
	locationDirectives.insert(std::pair<std::string, ldSeter>("cgi", &locationBlock::setCgi));
	locationDirectives.insert(std::pair<std::string, ldSeter>("autoindex", &locationBlock::setDirListing));
	// locationDirectives.insert(std::pair<std::string, int>("upload_post", 1));

	// ** Server Directives DB
	serverDirectives.insert(std::pair<std::string, funcSeter>("root", &virtualServer::setRootDir));
	serverDirectives.insert(std::pair<std::string, funcSeter>("listen", &virtualServer::setEndpoint));
	serverDirectives.insert(std::pair<std::string, funcSeter>("server_name", &virtualServer::setServerName));
	serverDirectives.insert(std::pair<std::string, funcSeter>("max_client_body_size", &virtualServer::setMaxBodySize));
	serverDirectives.insert(std::pair<std::string, funcSeter>("error_page", &virtualServer::setErrorPage));
}

configParser::~configParser()
{
	config_file.close();
}

std::pair<std::string, std::string>	configParser::locationSettings(const std::string& buff, size_t& i)
{
	size_t	end;
	size_t	pos;
	std::pair<std::string, std::string>	directive;

	pos = buff.find_first_of(" \t", i);
	end = buff.find_first_of("\n", pos);

	directive.first = buff.substr(i, pos - i);
	directive.second = buff.substr(pos, end - pos);
	i = end;
	return (directive);
}

size_t	configParser::parseLocationBlock(virtualServer& vs, const std::string& buffer)
{
	size_t								pos;
	std::string							path;
	std::pair<std::string, std::string>	directive;
	size_t		i = 0;

	i = Helpers::sepDistance(buffer, ' ') + 1;
	pos = buffer.find_first_of(" \t\n", i);
	path = buffer.substr(i, pos - i);
	i += path.length();
	for ( ; buffer[i] != '}'; i++)
	{
		if (buffer[i] == '\n' || buffer[i] == '{' \
			|| buffer[i] == 32 || buffer[i] == '\t') // ! skip spaces that before {
			continue ;
		directive = locationSettings(buffer, i);
		(vs.getLocations(path).*(locationDirectives[directive.first]))(directive.second);
	}
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
	int				blocks = 1;
	size_t			i = Helpers::sepDistance(buffer, '{') + 1;

	while (blocks && buffer[i]) // ** buffer'll never end before the blocks close (according to syntax), but you know :)
	{
		if (buffer[i] == '\n' && ++i)
			continue ;
		if (buffer[i] == '{' || buffer[i] == '}')
		{
			blocks += (1 - 2 * (buffer[i++] == '}'));
			continue ;
		}
		buffer.erase(0, i); // ** erase the first part eg. the srever keyword, \n..
		i = parseDirectives(vs, buffer, blocks);
		buffer.erase(0, i); // ** erase what we parse
		i = 0;
	}
	// std::cout << "**" << vs.getLocations("/") << "**" << std::endl;
	buffer.erase(0, i);
	return (vs);
}

std::vector<virtualServer>	configParser::parseConfiguration(void)
{
	std::vector<virtualServer>	virtualServers;
	configSyntax				syntaxChecker;
	std::string					buffer;
	size_t						skippedLines;

	try{
		buffer = syntaxChecker.syntaxEvaluation(config_file);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return (virtualServers);
	}
	while (!buffer.empty())
	{
		virtualServers.push_back(parseServerBlock(buffer));
		skippedLines = buffer.find_first_not_of("\n");
		// ** skipped new line that stay in the string to avoid extra call
		buffer.erase(0, skippedLines);
	}
	return (virtualServers);
}