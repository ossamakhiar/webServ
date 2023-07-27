/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configSyntax.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:32:02 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/27 22:12:24 by okhiar           ###   ########.fr       */
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
	bool	checkServerLine(const std::vector<std::string>& tokens, size_t i);
	bool	checkServerDircs(const std::vector<std::string>& tokens, size_t i);
	void	serverBlock(const std::vector<std::string>&, size_t, int&);
	bool	checkDirectives(const std::vector<std::string>& tokens, size_t i); // ** location directives
	bool	directiveOperations(std::map<std::string, int>::iterator it, const std::string& token, size_t size);
	bool	locationLine(const std::vector<std::string>&);
	void	locationBlock(const std::vector<std::string>&, size_t, int);
	void	checkBlock(const std::string& line, int& blocks);

public:
	configSyntax();
	~configSyntax();

	std::string syntaxEvaluation(std::ifstream& config_file);
};

#endif