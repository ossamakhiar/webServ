/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 22:00:20 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/11 15:45:50 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationBlock.hpp"

locationBlock::locationBlock()
{
	autoindex = false;
	root = "";
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
	autoindex = rhs.autoindex;
	allowed_methods = rhs.allowed_methods;
	index = rhs.index;
	root = rhs.root;
	cgi = rhs.cgi;
	_upload_post = rhs._upload_post;
	return (*this);
}

void	locationBlock::setRoot(const std::string& r)
{
	this->root = Helpers::trim(r);
}

void	locationBlock::setAllowedMethods(const std::string& inpt)
{
	this->allowed_methods = Helpers::split(inpt, " \t");
	for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it)
		if (!(*it == "GET" || *it == "get" || *it == "POST" \
			|| *it == "post" || *it == "DELETE" || *it == "delete"))
			throw std::runtime_error("error: bad method");
}

void	locationBlock::setCgi(const std::string& inpt)
{
	std::vector<std::string>	tokens;

	tokens = Helpers::split(inpt, " \t");
	if (access(tokens[1].c_str(), F_OK | X_OK))
		throw std::runtime_error("error: cgi-bin not excutable");
	cgi.insert(std::pair<std::string, std::string>(tokens[0], tokens[1]));
}

void	locationBlock::setDirListing(const std::string& inpt)
{
	if (Helpers::trim(inpt) == "off" || Helpers::trim(inpt) == "OFF")
		autoindex = false;
	else if (Helpers::trim(inpt) == "on" || Helpers::trim(inpt) == "ON")
		autoindex = true;
	else
		throw std::runtime_error("error: bad directory listing argument");
}

void	locationBlock::setIndex(const std::string& inpt)
{
	std::vector<std::string>	tokens;

	tokens = Helpers::split(inpt, " \t");
	// if (!tokens.size())
	// 	throw std::runtime_error("error: index empty")
	index = tokens;
}

void	locationBlock::setUploadPost(const std::string& str)
{
	std::string	upload;

	upload = Helpers::trim(str);
	if (upload[upload.length() - 1] != '/')
		upload.append("/");
	if (!PathVerifier::is_directory(upload))
		throw std::runtime_error("Upload post is not directory -_-");
	// std::cout << upload << std::endl;
	_upload_post = upload;
}

// TODO :: Getters

bool	locationBlock::get_autoindex(void) const
{
	return (autoindex);
}

const std::string& locationBlock::getRoot(void) const
{
	return (root);
}

const std::vector<std::string>&	locationBlock::getAllowedMethods() const
{
	return (allowed_methods);
}

const std::vector<std::string>&	locationBlock::getIndexs(void) const
{
	return (index);
}

const std::string&	locationBlock::getUploadPost(void) const
{
	return (_upload_post);
}


// ****************END*********

std::ostream& operator<<(std::ostream& os, const locationBlock& vs)
{
	os << "\t\troot: 		*" << vs.root;
	os << "\n\t\tautoindex: " << (vs.autoindex ? "ON" : "OFF");
	os << "\n\t\tallowed_methods: ";
	for (std::vector<std::string>::const_iterator it = vs.allowed_methods.begin(); it != vs.allowed_methods.end(); ++it)
		os << *it << " ";
	os << "\n\t\tcgi:" << vs.cgi.size();
	for (std::map<std::string, std::string>::const_iterator it = vs.cgi.begin(); it != vs.cgi.end(); ++it)
		os << "\n\t\t\t" << it->first << ": " << it->second;
	return (os);
}