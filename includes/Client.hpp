/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:42:59 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/12 15:53:07 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "virtualServer.hpp"
#include "requestMessage.hpp"
#include "Response.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

enum e_client_states
{
	READING_REQUEST,
	BUILD_RESPONSE,
	CLIENT_DONE,
	DISCONNECTED
};

class Client
{
private:
	int							client_state;
	int							client_socket;
	struct sockaddr_in			client_addr;

	std::vector<virtualServer*>	&_vs_endpoint;
	virtualServer				*_vs;// ** vServer That client communicate to
	locationBlock				*_location; // ** set the related location to a URI if it's exist..
	std::string					_path;

	requestMessage				_request;
	e_status_code				_status_code;

	Response					_response;

	Client(const Client& other);
	Client& operator=(const Client& rhs);
public:
	Client(int fd, struct sockaddr_in addr, std::vector<virtualServer*>	&);
	~Client();

	// TODO :: Setters
	void			setVS(virtualServer *vs);

	// TODO :: Getters
	virtualServer	*getVS(void) const;
	int				getState(void) const;
	void			responseImportantSettings(void);

	void	readRequest(void);
	void	makeResponse(void);
};

#endif