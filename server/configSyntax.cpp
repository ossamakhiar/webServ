/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configSyntax.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:35:43 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 21:52:04 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configSyntax.hpp"

configSyntax::configSyntax()
{
	// ** Location Directives DB
	locationDirectives.insert(std::pair<std::string, int>("allowed_methods", 3));
	locationDirectives.insert(std::pair<std::string, int>("autoindex", 1));
	locationDirectives.insert(std::pair<std::string, int>("root", 1));
	locationDirectives.insert(std::pair<std::string, int>("cgi", 2));
	locationDirectives.insert(std::pair<std::string, int>("upload_post", 1));

	// ** Server Directives DB
	serverDirectives.insert(std::pair<std::string, int>("listen", 1));
	serverDirectives.insert(std::pair<std::string, int>("root", 1));
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

bool	configSyntax::checkDirectives(const std::string& directive, size_t size)
{
	std::map<std::string, int>::iterator itlow;

	if (!locationDirectives.count(directive))
		return (false);
	itlow = locationDirectives.lower_bound(directive);
	return (directiveOperations(itlow, directive, size));
}

bool	configSyntax::checkServerDircs(const std::string& directive, size_t size)
{
	std::map<std::string, int>::iterator	itlow;

	itlow = serverDirectives.lower_bound(directive);
	if (!serverDirectives.count(directive) || \
		size != static_cast<size_t>(itlow->second) + 1)
		return (false);
	return (true);
}

size_t	configSyntax::checkLocationStart(const std::string& buff, size_t start)
{
	size_t	i;
	int		state;

	state = 0;
	for (i = start; buff[i] && buff[i] != '{'; i++)
	{
		if (buff[i] == 32 || buff[i] == '\t' || buff[i] == '\n')
		{
			if (state == 1)
				state = 2;
			continue ;
		}
		if (state == 2)
			throw std::runtime_error("syntax error: bad location start block");
		state = 1;
	}
	if (!state || buff[i + 1] != '\n')
		throw std::runtime_error("syntax error: bad location start block");
	return (++i);
}

void	configSyntax::locationBlock(std::string& buff, size_t& i)
{
	int							block = 1;
	size_t						pos;
	std::string					directive_line;	
	std::vector<std::string>	tokens;

	i += 8;// * skip length of location
	i = checkLocationStart(buff, i);
	while (block && buff[i])
	{
		if (buff[i] == '\n')
		{
			i++;
			continue ;
		}
		pos = buff.find_first_of("\n", i);
		directive_line = buff.substr(i, pos - i);
		tokens = Helpers::split(directive_line, " \t");
		if (tokens[0] == "}")
		{
			(block = 0, i++);
			continue ;
		}
		if (!checkDirectives(tokens[0], tokens.size()))
			throw std::runtime_error("syntax error: bad location directive");
		i += directive_line.length();
	}
	if (block)
		throw std::runtime_error("syntax error: unclosed braces");
}

size_t	configSyntax::serverBlockStart(const std::string& buff, size_t start)
{
	size_t	i;

	for (i = start; buff[i] && buff[i] != '{'; i++)
	{
		if (buff[i] == 32 || buff[i] == '\t' || buff[i] == '\n')
			continue ;
		break ;
	}
	if (buff[i] != '{' || buff[i + 1] != '\n')
		throw std::runtime_error("syntax error: bad server block start");
	return (i + 2); // ! skip { and \n
}

void	configSyntax::checkBlock(std::string& buff, size_t& i)
{
	int							block  = 1;
	std::string					directive_line;
	size_t						pos = buff.find_first_of(" \t\n{", i);
	std::vector<std::string>	tokens;

	if (buff.substr(i, pos - i) != "server")
		throw std::runtime_error("syntax error: server block error");
	i = serverBlockStart(buff, i + 6);
	while (block && buff[i])
	{
		if (buff[i] == '\n')
		{
			i++;
			continue ;
		}
		pos = buff.find_first_of("\n", i);
		directive_line = buff.substr(i, pos - i);
		tokens = Helpers::split(directive_line, " \t");
		if (tokens[0] == "}")
			(block = 0, i++);
		else if (tokens[0] == "location")
			locationBlock(buff, i);
		else
		{
			if (!checkServerDircs(tokens[0], tokens.size()))
				throw std::runtime_error("syntax error: bad server directive");
			i += directive_line.length();
		}
	}
	if (block)
		throw std::runtime_error("syntax error: unclosed braces");
}

std::string	configSyntax::syntaxEvaluation(std::ifstream& config_file)
{
	std::string	current_line;
	std::string	buffer;

	buffer = "";
	while (std::getline(config_file, current_line))
	{
		current_line = Helpers::trim(current_line);
		if (isComment(current_line))
			continue ;
		buffer += current_line;
		buffer += "\n";
	}
	for (size_t i = 0; i < buffer.length();)
	{
		checkBlock(buffer, i);
		if (buffer[i] == '\n')
			i++;
	}
	return (buffer);
}