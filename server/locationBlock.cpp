/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 22:00:20 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/25 12:44:57 by okhiar           ###   ########.fr       */
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
	this->root = Helpers::trim(r);
}

void	locationBlock::setAllowedMethods(const std::string& inpt)
{
	std::vector<std::string> allowed_set;

	allowed_set.push_back("GET");
	allowed_set.push_back("POST");
	allowed_set.push_back("DELETE");
	this->allowed_methods = Helpers::split(inpt, " \t");
	for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it)
		if (!Helpers::elemInside(*it, allowed_set.begin(), allowed_set.end()))
			throw std::runtime_error("error: bad method");
}

void	locationBlock::setCgi(const std::string& inpt)
{
	std::vector<std::string>	tokens;

	tokens = Helpers::split(inpt, " \t");
	cgi.insert(std::pair<std::string, std::string>(tokens[0], tokens[1]));
	for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}


std::ostream& operator<<(std::ostream& os, const locationBlock& vs)
{
	os << "\t\troot: 		*" << vs.root;
	os << "\n\t\tallowed_methods: ";
	for (std::vector<std::string>::const_iterator it = vs.allowed_methods.begin(); it != vs.allowed_methods.end(); ++it)
		os << *it << " ";
	os << "\n\t\tcgi:" << vs.cgi.size();
	for (std::map<std::string, std::string>::const_iterator it = vs.cgi.begin(); it != vs.cgi.end(); ++it)
		os << "\n\t\t\t" << it->first << ": " << it->second;
	return (os);
}