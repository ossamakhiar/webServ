/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:38:04 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/30 15:56:38 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_HPP
# define HELPERS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class Helpers
{
private:
	Helpers();
	Helpers(const Helpers&);
	Helpers& operator=(const Helpers&);

public:
	static void				retCheck(int ret, const std::string& msg);
	static std::string	trim(const std::string&);
	static size_t		sepDistance(const std::string&, const char&);
	static bool			findElement(std::vector<std::string> vec, const std::string& elem);
	static int			safeAtoi(const std::string&);
	static std::vector<std::string>	split(std::string str, const std::string& charset);

	template<typename T, typename Iterator>
	static bool	elemInside(T elem, Iterator start, Iterator end)
	{
		while (start != end)
		{
			if (*start == elem)
				return (true);
			start++;
		}
		return (false);
	}


	class exceptionError : public std::exception
	{
		private:
			std::string	msg;

		public:
			exceptionError() : msg("error!") {
				
			}
			exceptionError(const std::string& m) : msg(m) {
				
			}
			const char* what() const throw()
			{
				return (msg.c_str());
			}
			virtual ~exceptionError() throw() {};
	};
};

#endif