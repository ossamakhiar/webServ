/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferQueuing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 17:16:08 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/06 21:57:28 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bufferQueuing.hpp"

// ? Transfer encoding chunked, helper

bufferQueuing::bufferQueuing() : size_extracted(false), start(0), size(0), last_chunk(false)
{
	
}

bufferQueuing::~bufferQueuing()
{

}

bufferQueuing::bufferQueuing(const bufferQueuing& other)
{
	*this = other;
}

bufferQueuing&	bufferQueuing::operator=(const bufferQueuing& rhs)
{
	if (this == &rhs)
		return (*this);
	buffer_queue = rhs.buffer_queue;
	size = rhs.size;
	return (*this);
}

// * Getter
const unsigned char*	bufferQueuing::getChunks(void) const
{
	return (data.data());
}

size_t	bufferQueuing::chunk_size(void) const
{
	return (data.size());
}

bool	bufferQueuing::isLastChunk(void) const
{
	return (last_chunk);
}

void	bufferQueuing::clear_data(void)
{
	data.clear();
}

// ***************

void	bufferQueuing::bufferFeed(const char *buffer, int bytes)
{
	// std::cout << buffer << std::endl;
	for (size_t i = 0; i < static_cast<size_t>(bytes); ++i)
		buffer_queue.push_back(buffer[i]);
}

size_t	bufferQueuing::extractSize(void)
{
	size_t	i = start;
	std::string	hexa = "";

	while (buffer_queue[i] != '\r' && i < buffer_queue.size())
	{
		hexa += buffer_queue[i];
		i++;
	}
	if (buffer_queue[i] != '\r' || (i + 1 >= buffer_queue.size() || \
				buffer_queue[i + 1] != '\n'))
		throw (1);
	start = i + 2; // ! skip length of hexa and CRLF
	size_extracted = true;
	return (Helpers::hexaToInteger(hexa));
}

// ** .... waiting for idea........
void	bufferQueuing::processAndShiftData(void)
{
	size_t	i = 0;

	if (!size_extracted)
	{
		// if (!start)
		// 	std::cout << "here.........\n";
		try {
			size = extractSize(); // ? back here
		} catch (...) {
			return ;
		}
	}
	// std::cout << "\e[1;32mSIZE\e[0m: " << size << std::endl;
	if (size == 0){
		last_chunk = true;
		size_extracted = false;
		return ; 
	}
	if ((buffer_queue.size() - start - 2 < size)) // ! if not possible to extract the the whole chunk
		return ; // * POP
	while(i < size)
	{
		// std::cout << buffer_queue[start + i] << std::endl;
		data.push_back(buffer_queue[start + i]);
		i++;
		// * push back to return buffer (user buffer)
	}
	// hint : clear from 0 to the size that read
	start += size + 2;
	size_extracted = false;
	processAndShiftData();
}
