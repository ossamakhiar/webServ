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

int			Helpers::safeAtoi(const std::string& str)
{
	char	*eptr;
	long	res;

	res = std::strtol(str.c_str(), &eptr, 10);
	if (*eptr != '\0')
		throw std::runtime_error("atoi: bad argument");
	if (res > INT_MAX || res < INT_MIN)
		throw std::runtime_error("atoi: can't convert");
	return (res);
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

void	Helpers::retCheck(int ret, const std::string& msg)
{
	if (ret >= 0)
		return ;
	std::cout << "here...\n";
	throw Helpers::exceptionError(msg);
}

std::string	Helpers::strTolower(const std::string& str)
{
	size_t i = -1;
	std::string	ret;

	ret = str;
	while (ret[++i])
		if (ret[i] >= 'A' && ret[i] <= 'Z')
			ret[i] += 32;
	return (ret);
}

// ! make this more smarter
char			Helpers::hexaToDecimal(const std::string& hex)
{
	int	digit1 = 0;
	int	digit2 = 0;

	if (hex[0] <= '9' && hex[0] >= '0')
		digit1 = hex[0] - 48;
	if (hex[0] >= 'a' && hex[0] <= 'f')
		digit1 = hex[0] - 87;
	if (hex[1] <= '9' && hex[1] >= '0')
		digit2 = hex[1] - 48;
	if (hex[1] >= 'a' && hex[1] <= 'f')
		digit2 = hex[1] - 87;
	digit1 *= 16;
	return (digit1 + digit2);
}
