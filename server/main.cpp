/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:54:41 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/23 21:17:01 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParser.hpp"

int	main(void)
{
	configParser	parser("./config/test.conf");

	try {
		parser.parseConfiguration();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}