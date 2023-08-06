/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftString.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 16:44:06 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/06 18:39:36 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftString.hpp"

ftString::ftString() : _str(new char[1]), _size(0)
{
	*_str = 0;	
}

ftString::~ftString()
{
	delete[] (_str);
}

ftString::ftString(const char* str)
{
	size_t i;
	size_t	len = strlen(str);

	_size = len;
	_str = new char[len + 1];
	for (i = 0; i < len; i++)
		_str[i] = str[i];
	_str[i] = 0;
}

ftString::ftString(const ftString& other) : _str(NULL)
{
	*this = other;
}

ftString&	ftString::operator=(const ftString& rhs)
{
	if (this == &rhs)
		return (*this);
	_size = rhs._size;
	if (_str)
		delete[] _str;
	_str = new char[_size + 1];
	memcpy(_str, rhs._str, _size + 1);
	return (*this);
}

bool	ftString::operator==(const ftString& rhs) const
{
	size_t	i = 0;

	while (i < _size && i < rhs._size)
	{
		if (_str[i] != rhs._str[i])
			return (false);
		i++;
	}
	return (i >= _size && i >= rhs._size);
}

bool	ftString::operator!=(const ftString& rhs) const
{
	return (!(*this == rhs));
}

ftString&	ftString::operator+=(const ftString& rhs)
{
	return (this->append(rhs));
}

char&	ftString::operator[](size_t pos) const
{
	return (_str[pos]);
}

char&	ftString::at(size_t pos) const
{
	if (pos >= _size)
		throw std::runtime_error("out of range");
	return (_str[pos]);
}

const char*	ftString::c_str(void) const
{
	return (_str);
}

size_t	ftString::size(void) const
{
	return (_size);
}

size_t	ftString::strlen(const char *str)
{
	size_t i = 0;

	while (str[i])
		i++;
	return (i);
}

ftString&	ftString::append(const ftString& other)
{
	char	*tmp;

	tmp = new char[other._size + _size + 1];
	memcpy(tmp, _str, _size);
	memcpy(tmp + _size, other._str, other._size + 1);
	delete[] _str;
	_str = tmp;
	_size += other._size;
	return (*this);
}

// ? this substr version does not check for \0
ftString	ftString::substr(size_t pos, size_t len)
{
	ftString	ret;
	size_t	i = 0;

	if (pos >= _size)
		throw std::runtime_error("out of range");
	if (len > _size)
		len = _size - pos;
	delete[] (ret._str);
	ret._str = new char[len + 1];
	while (i < len)
	{
		ret._str[i] = _str[i + pos];
		i++;
	}
	ret._str[i] = '\0';
	ret._size = len;
	return (ret);
}

size_t		ftString::find(const char* str, size_t pos)
{
	size_t	len = strlen(str);

	if (pos >= _size || len > _size - pos)
		return (npos);
	for (size_t i = pos; i <= _size - len; i++)
	{
		size_t j;
		for (j = 0; j < len; j++)
			if (_str[i + j] != str[j])
				break ;
		if (j == len)
			return (i);
	}
	return (npos);
}




std::ostream& operator<<(std::ostream &os, const ftString& str)
{
	os << str.c_str();
	return (os);
}