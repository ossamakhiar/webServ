/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:38:04 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/23 14:03:41 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_HPP
# define HELPERS_HPP

#include <iostream>
#include <string>
#include <vector>

class Helpers
{
private:
	Helpers();
	Helpers(const Helpers&);
	Helpers& operator=(const Helpers&);

public:
	static std::string	trim(const std::string&);
	static size_t		sepDistance(const std::string&, const char&);
	static bool			findElement(std::vector<std::string> vec, const std::string& elem);
	static std::vector<std::string>	split(std::string str, const std::string& charset);
};

#endif