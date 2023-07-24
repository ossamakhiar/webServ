/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 22:00:20 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/24 14:16:35 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationBlock.hpp"

locationBlock::locationBlock()
{

}

locationBlock::~locationBlock()
{

}

locationBlock::locationBlock(const locationBlock& other)
{
	*this = other;
}

locationBlock&	locationBlock::operator=(const locationBlock& rhs)
{
	if (this == &rhs)
		return (*this);
	directory_listing = rhs.directory_listing;
	allowed_methods = rhs.allowed_methods;
	index = rhs.index;
	root = rhs.root;
	return (*this);
}


void	locationBlock::setRoot(const std::string& r)
{
	this->root = r;
}

void	locationBlock::setAllowedMethods(const std::string& inpt)
{
	this->allowed_methods = Helpers::split(inpt, " \t");
}


std::ostream& operator<<(std::ostream& os, const locationBlock& vs)
{
	os << "\t\troot: 		" << vs.root;
	os << "\n\t\tallowed_methods: ";
	for (std::vector<std::string>::const_iterator it = vs.allowed_methods.begin(); it != vs.allowed_methods.end(); ++it)
		os << *it << " ";
	return (os);
}