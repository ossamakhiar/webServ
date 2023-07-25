/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:54:18 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/25 12:48:24 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "helpers.hpp"

class locationBlock
{
private:
	bool								directory_listing;
	std::string							root;
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

	std::map<std::string, std::string> getCGI(void)
	{
		return (cgi);
	}

	friend std::ostream& operator<<(std::ostream& os, const locationBlock& vs);
};

std::ostream& operator<<(std::ostream& os, const locationBlock& vs);

#endif