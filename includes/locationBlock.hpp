/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:54:18 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/11 15:44:55 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "PathVerifier.hpp"
#include "helpers.hpp"

class locationBlock
{
private:
	bool								autoindex;
	std::string							root;
	std::string							_upload_post;
	std::vector<std::string>			allowed_methods;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi;

public:
	locationBlock();
	locationBlock(const locationBlock&);
	locationBlock& operator=(const locationBlock&);
	~locationBlock();

	void	setRoot(const std::string&);
	void	setAllowedMethods(const std::string&);
	void	setCgi(const std::string&);
	void	setDirListing(const std::string&);
	void	setIndex(const std::string&);
	void	setUploadPost(const std::string&);

	// TODO :: Getters
	const std::string&				getRoot(void) const;
	bool							get_autoindex(void) const;
	const std::vector<std::string>&	getAllowedMethods(void) const;
	const std::vector<std::string>&	getIndexs(void) const;
	const std::string&				getUploadPost(void) const;

	std::map<std::string, std::string>& getCGI(void)
	{
		return (cgi);
	}


	friend std::ostream& operator<<(std::ostream& os, const locationBlock& vs);
};

std::ostream& operator<<(std::ostream& os, const locationBlock& vs);

#endif