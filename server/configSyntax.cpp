/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configSyntax.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:35:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/22 21:34:13 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configSyntax.hpp"

configSyntax::configSyntax()
{
	// ** Location Directives DB
	locationDirectives.insert(std::pair<std::string, int>("allowed_methods", 3));
	locationDirectives.insert(std::pair<std::string, int>("directory_listing", 1));
	locationDirectives.insert(std::pair<std::string, int>("root", 1));
	locationDirectives.insert(std::pair<std::string, int>("cgi", 2));
	locationDirectives.insert(std::pair<std::string, int>("upload_post", 1));

	// ** Server Directives DB
	serverDirectives.insert(std::pair<std::string, int>("listen", 1));
	serverDirectives.insert(std::pair<std::string, int>("server_name", 2));
	serverDirectives.insert(std::pair<std::string, int>("max_client_body_size", 1));
	serverDirectives.insert(std::pair<std::string, int>("error_page", 2));
}

configSyntax::~configSyntax()
{

}

bool	configSyntax::isComment(const std::string& line)
{
	return (line[0] == '#' || line.empty());
}

bool	configSyntax::locationLine(const std::vector<std::string>& tokens)
{
	return (tokens.size() == (2 + (tokens.back() == "{")));
}

bool	configSyntax::directiveOperations(std::map<std::string, int>::iterator it, const std::string& token, size_t size)
{
	if (token != "allowed_methods")
		if (token != it->first || size != static_cast<size_t>(it->second) + 1)
			return (false);
	if (token == "allowed_methods")
		if (size > (it->second + static_cast<size_t>(1)) || size == 1)
			return (false);
	return (true);
}

bool	configSyntax::checkDirectives(const std::vector<std::string>& tokens, size_t i)
{
	std::map<std::string, int>::iterator itlow;

	if (i != 0)
		return (true);
	itlow = locationDirectives.lower_bound(tokens[i]);
	return (directiveOperations(itlow, tokens[i], tokens.size()));
}

void	configSyntax::locationBlock(const std::vector<std::string>& tokens, size_t i, int blocks)
{
	if ((blocks != 1 && tokens[i] == "location") || (!locationLine(tokens) && blocks != 2))
		throw std::runtime_error("syntax error: bad location block level");
	if (blocks == 2 && !checkDirectives(tokens, i))
		throw std::runtime_error("syntax error: bad location directive");
}

bool	configSyntax::checkServerDircs(const std::vector<std::string>& tokens, size_t i)
{
	std::map<std::string, int>::iterator	itlow;

	if (i != 0)
		return (true);
	// std::cout << tokens[i] << std::endl;
	itlow = serverDirectives.lower_bound(tokens[i]);
	if (itlow->first != tokens[i] || tokens.size() != static_cast<size_t>(itlow->second) + 1)
		return (false);
	return (true);
}

void	configSyntax::serverBlock(const std::vector<std::string>& tokens, size_t i, int& blocks)
{
	static int block_type;

	if (tokens[i] == "server" && blocks != 0)
		throw std::runtime_error("syntax error: bad server block level");
	if (tokens[i] == "{" && block_type == LOCATION_BLOCK)
		blocks++;
	else if (tokens[i] == "}" && block_type == LOCATION_BLOCK)
	{
		block_type = 0x0;
		blocks--;
	}
	else if ((tokens[i] == "location" && (block_type = LOCATION_BLOCK)) || block_type == LOCATION_BLOCK)
		locationBlock(tokens, i, blocks);
	else if (blocks == 1 && !checkServerDircs(tokens, i))
		throw std::runtime_error("syntax error: bad server directive");
	// ? else (directive) ==> handling
}

void	configSyntax::checkBlock(const std::string& line, int& blocks)
{
	std::vector<std::string>	tokens;
	static int					block_type;

	tokens = Helpers::split(line, " \t");
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "{" && blocks == 0)
			blocks++;
		else if (tokens[i] == "}" && blocks == 1)
		{
			block_type = 0x0;
			blocks--;
		}
		else if ((tokens[i] == "server" && (block_type = SERVER_BLOCK)) || block_type == SERVER_BLOCK)
			serverBlock(tokens, i, blocks);
		else
			throw std::runtime_error("syntax error");
	}
}

std::string	configSyntax::syntaxEvaluation(std::ifstream& config_file)
{
	std::string	buffer;
	std::string	current_line;
	int			blocks = 0;

	buffer = "";
	while (std::getline(config_file, current_line))
	{
		current_line = Helpers::trim(current_line);
		if (isComment(current_line))
			continue ;
		checkBlock(current_line, blocks);
		buffer += current_line;
		buffer += "\n";
	}
	if (blocks)
		throw std::runtime_error("syntax error");
	return (buffer);
}