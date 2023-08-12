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
		str.erase(0, pos + 1); // * plus one to erase the delimeter also
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

int	Helpers::getDigit(const char& c)
{
	if (c <= 57 && c >= 48)
		return (c - 48);
	if (c < 97 || c > 102)
		throw std::runtime_error("hexa-to-int: bad input");
	return (c - 87);
}

unsigned int	Helpers::hexaToInteger(const std::string& hex)
{
	unsigned int	result;
	int				digit;
	size_t			i;
	char			c;

	i = result = 0;
	while (i < hex.length())
	{
		c = hex[i];
		if (c >= 65 && c <= 90)
			c += 32;
		digit = getDigit(c);
		result *= 16; // * haxadecimal Base;
		result += digit;
		i++;
	}
	return (result);
}

std::string	Helpers::randomFileNameGen(void)
{
	std::string	alphanums = "_";
	std::string	file_name;

	std::srand(time(NULL));
	for (size_t i = 0; i < 26; ++i)
	{
		if (i <= 9)
			alphanums += (i + 48);
		alphanums += (i + 97);
		alphanums += (i + 65);
	}
	// ! BASE = 26 + 26 + 10 + 1 = 63
	for (size_t i = 0; i < 10; i++)
		file_name += alphanums[rand() % 63];
	return (file_name);
}

std::string	Helpers::precent_decoding(const std::string& encoded)
{
	size_t		i = 0;
	std::string	decoded;

	while (i < encoded.length())
	{
		if (encoded[i] != '%')
		{
			decoded += encoded[i++];
			continue ;
		}
		i++;
		decoded += Helpers::hexaToDecimal(Helpers::strTolower(encoded.substr(i, 2)));
		i += 2;
	}
	return (decoded);
}

void	Helpers::readFile(std::ifstream& is, ftString& str)
{
    is.seekg(0, std::ios::end); // ? read about this
    std::streamsize size = is.tellg();
    is.seekg(0, std::ios::beg);

    str.clear();
    str.resize(size);
	std::cout << "SIZEEEEe: " << size << std::endl;
    is.read(&str[0], size);
}