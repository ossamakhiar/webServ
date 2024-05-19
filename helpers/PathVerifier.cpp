/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathVerifier.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 11:32:07 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/10 12:05:59 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PathVerifier.hpp"
#include <iostream>

bool	PathVerifier::is_file(const std::string& path)
{
	struct stat fileStat;

	if (stat(path.c_str(), &fileStat) == 0)
        return S_ISREG(fileStat.st_mode);
    return (false);
}

bool PathVerifier::is_directory(const std::string& path)
{
	struct stat fileStat;

	if (stat(path.c_str(), &fileStat) == 0)
        return S_ISDIR(fileStat.st_mode);
    return (false);
}

bool PathVerifier::path_exists(const std::string& path)
{
    struct stat fileStat;
	// std::cout << "Checking for " << path << "/*-/-*/-*/-/\n";
    return (stat(path.c_str(), &fileStat) == 0);
}

std::string	PathVerifier::get_file_ext(const std::string& path)
{
	size_t	pos;

	pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return ("");
	return (path.substr(pos, path.length() - pos));
}
