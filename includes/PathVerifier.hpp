/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathVerifier.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 11:29:46 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/10 11:42:39 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PATHVERIFIER_HPP
# define PATHVERIFIER_HPP

#include <string>
#include <sys/stat.h>

class PathVerifier
{
private:
	PathVerifier();

public:
	static bool is_file(const std::string&);
	static bool is_directory(const std::string&);
	static bool	path_exists(const std::string&);

	static std::string	get_file_ext(const std::string&);
};

#endif