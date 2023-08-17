/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:06:53 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/17 19:25:59 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::vector<std::string> Response::cgi_env_setting(void)
{
	// char	**env = NULL;	_stored_type = EXTERNAL_STORAGE;
	std::vector<std::string>	env;

	// cgi Env setting
	env.push_back("REQUEST_METHOD=" + _request_method);
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("QUERY_STRING=" + _req->getQuery());
    env.push_back("SCRIPT_NAME=" + getScriptName());
    env.push_back("PATH_INFO=" + _req->getPath());
    env.push_back("SCRIPT_FILENAME=" + _req->getPhysicalPath());
    env.push_back("REDIRECT_STATUS=200");
    env.push_back("REQUEST_URI=" + _req->getURI());

	for (std::map<std::string, std::string>::const_iterator it = _req->getHeaderFields().begin(); it != _req->getHeaderFields().end(); ++it)
	{
		std::string	key;

		key = it->first;
		key = Helpers::strToUpper(key);
		key = Helpers::findReplace(key, '-', '_'); // ! problem here..
		key = "HTTP_" + key;
		key += "=";
		key += it->second;
		env.push_back(key);
	}

	return (env);
}

void	Response::redirect_cgi_output()
{
	int	fd;

	fd = open(_cgi_outfile.c_str(), O_CREAT | O_WRONLY, 0666);

	if (dup2(fd, 1) == -1)
		exit(EXIT_FAILURE);
}

void	Response::cgi_handler(void)
{
	char						**cgi_env;
	char						*cgi_args[3];
	std::vector<std::string>	env_vec;

	env_vec = cgi_env_setting();
	cgi_env = new char *[env_vec.size() + 1];
	for (size_t i = 0; i < env_vec.size(); ++i)
		cgi_env[i] = (char *)env_vec[i].c_str();
	cgi_env[env_vec.size() - 1] = NULL;
	cgi_args[0] = (char *)_location->getCGI().at(_cgi_ext).c_str();
	cgi_args[1] = (char *)_req->getPhysicalPath().c_str();
	cgi_args[2] = NULL;

	_cgi_outfile = "/tmp/" + Helpers::randomFileNameGen();

	_cgi_pid = fork();
	if (_cgi_pid != 0)
		usleep(100);
	if (!_cgi_pid)
	{
		if (_request_method == "POST")
		{
			// dup2 cgi input to the post body file
		}
		redirect_cgi_output();
		execve(cgi_args[0], cgi_args, cgi_env);
		exit(EXIT_FAILURE);
	}

	delete[] cgi_env;
	_responsing_state = CGI_WAITING;
}

