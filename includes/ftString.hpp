/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftString.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 16:28:57 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/11 19:59:44 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTSTRING_HPP
# define FTSTRING_HPP

// #include "helpers.hpp"
#include <string.h>
#include <iostream>

class ftString
{
private:
	char	*_str;
	size_t	_size;

	size_t strlen(const char *) const;

public:
	static const size_t npos = -1;

	ftString();
	ftString(const char*);
	ftString(const char*, size_t);
	ftString(const std::string& str);
	ftString(const ftString&);
	ftString&	operator=(const ftString&);
	ftString&	operator=(const std::string& rhs);
	~ftString();

	const char*	c_str(void) const;

	ftString&	append(const ftString&);
	ftString&	append(const char* str, size_t len);
	ftString	substr(size_t pos, size_t len = npos);
	size_t		size(void) const;
	size_t		length(void) const;
	void		clear(void);
	void		resize(size_t n);

	size_t		find(const char* str, size_t pos = 0) const;

	char&	at(size_t pos) const;

	bool	operator==(const ftString&) const;
	bool	operator!=(const ftString&) const;
	char&	operator[](size_t pos) const;

	ftString&	operator+=(const ftString& rhs);
};

std::ostream& operator<<(std::ostream &os, const ftString& str);

#endif