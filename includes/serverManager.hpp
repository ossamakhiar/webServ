/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:57:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/07/27 22:00:36 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "virtualServer.hpp"

class serverManager
{
private:
	std::vector<virtualServer>	_virtual_servers;

	// ** private member function

public:
	serverManager();
	~serverManager();
	serverManager(const serverManager&);
	serverManager& operator=(const serverManager&);

	
	
}

#endif