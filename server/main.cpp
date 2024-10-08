/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:54:41 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/02 22:16:53 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverManager.hpp"

int	main(void)
{
	// std::vector<virtualServer>	virtualServers;
	// configParser	parser("./config/test.conf");

	// try {
	// 	virtualServers = parser.parseConfiguration();
	// 	std::cout << virtualServers.size() << std::endl;
	// 	for (std::vector<virtualServer>::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it)
	// 		std::cout << *it << std::endl;
	// } catch (std::exception& e) {
	// 	std::cout << e.what() << std::endl;
	// }
	// system("leaks webserv");
	try {
		serverManager	manager;
		manager.IOSynchronous();
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}