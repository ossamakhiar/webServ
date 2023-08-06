/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftString.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 16:28:57 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/06 17:55:48 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTSTRING_HPP
# define FTSTRING_HPP

// #include "helpers.hpp"
#include <iostream>

class ftString
{
private:
	char	*_str;
	size_t	_size;

	size_t strlen(const char *);

public:
	static const size_t npos = -1;

	ftString();
	ftString(const char*);
	ftString(const char*, size_t);
	ftString(const ftString&);
	ftString&	operator=(const ftString&);
	~ftString();

	const char*	c_str(void) const;

	ftString&	append(const ftString&);
	ftString	substr(size_t pos, size_t len = npos);
	size_t		size(void) const;

	size_t		find(const char* str, size_t pos = 0);

	char&	at(size_t pos) const;

	bool	operator==(const ftString&) const;
	bool	operator!=(const ftString&) const;
	char&	operator[](size_t pos) const;
};

std::ostream& operator<<(std::ostream &os, const ftString& str);

#endif