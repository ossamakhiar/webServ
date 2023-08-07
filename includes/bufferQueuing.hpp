/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferQueuing.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 15:53:22 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/07 16:00:03 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFERQUEUING_HPP
# define BUFFERQUEUING_HPP

#include "helpers.hpp"
#include "ftString.hpp"


class bufferQueuing
{
private:
	bool						size_extracted;
	size_t						start;
	size_t						size;

	ftString	buffer_queue;
	ftString	data;
	//std::vector<unsigned char>	buffer_queue;
	//std::vector<unsigned char>	data;

	bool						last_chunk;

	size_t	extractSize(void);

public:
	bufferQueuing();
	~bufferQueuing();
	bufferQueuing(const bufferQueuing&);
	bufferQueuing& operator=(const bufferQueuing&);

	/* // * remove the buffer that is written, and place the buffer that is readed
		* from by reading the FIX SIZE (next chunk) */
	void	processAndShiftData(void);
	void	bufferFeed(const char *buffer, int bytes);

	void	clear_data(void);


	const char*	getChunks() const;
	size_t					chunk_size() const;
	bool					isLastChunk() const;
};

#endif