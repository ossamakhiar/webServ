/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:40:55 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/21 21:44:09 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"

std::string	Helpers::trim(const std::string& str)
{
	size_t	start;
	size_t	last;

	start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	last = str.find_last_not_of(" \t");
	return (str.substr(start, last - start + 1));
}

bool			Helpers::findElement(std::vector<std::string> vec, const std::string& elem)
{
	for (size_t i = 0; i < vec.size(); i++)
		if (vec[i] == elem)
			return (true);
	return (false);
}

size_t		Helpers::sepDistance(const std::string& buff, const char& c)
{
	size_t	i;

	i = 0;
	while (buff[i] && buff[i] != c)
		i++;
	return (i);
}

std::vector<std::string>	Helpers::split(std::string str, const std::string& charset)
{
	std::vector<std::string>	tokens;
	std::string					token;
	size_t						pos;

	while ((pos = str.find_first_of(charset)) != std::string::npos)
	{
		token = str.substr(0, pos);
		str.erase(0, pos + 1);
		if (!token.empty())
			tokens.push_back(token);
	}
	if (!str.empty())
		tokens.push_back(str);
	return (tokens);
}