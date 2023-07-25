/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:54:41 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/25 13:57:34 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParser.hpp"

int	main(void)
{
	std::vector<virtualServer>	virtualServers;
	configParser	parser("./config/test.conf");

	try {
		virtualServers = parser.parseConfiguration();
		std::cout << virtualServers.size() << std::endl;
		for (std::vector<virtualServer>::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it)
			std::cout << *it << std::endl;
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}