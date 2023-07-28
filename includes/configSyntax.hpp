/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configSyntax.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:32:02 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/28 13:43:08 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSYNTAX_HPP
# define CONFIGSYNTAX_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "helpers.hpp"

enum e_blocks
{
	SERVER_BLOCK = 0x000000ff,
	LOCATION_BLOCK = 0x0000ff00
};


class configSyntax
{
private:
	std::map<std::string, int>	locationDirectives;
	std::map<std::string, int>	serverDirectives;


	configSyntax(const configSyntax&);
	configSyntax& operator=(const configSyntax&);

	// TODO :: private methods
	bool	isComment(const std::string&);
	bool	checkServerDircs(const std::string& directive, size_t size);
	size_t	serverBlockStart(const std::string& buff, size_t start);
	bool	checkDirectives(const std::string& directive, size_t size);
	// bool	checkDirectives(const std::vector<std::string>& tokens, size_t i); // ** location directives
	bool	directiveOperations(std::map<std::string, int>::iterator it, const std::string& token, size_t size);
	size_t	checkLocationStart(const std::string& buff, size_t start);
	bool	locationLine(const std::vector<std::string>&);
	void	locationBlock(std::string& buff, size_t& i);
	// void	locationBlock(const std::vector<std::string>&, size_t, int);
	void	checkBlock(std::string& buff, size_t& i);
	// void	checkBlock(const std::string& line, int& blocks);

public:
	configSyntax();
	~configSyntax();

	std::string syntaxEvaluation(std::ifstream& config_file);
};

#endif