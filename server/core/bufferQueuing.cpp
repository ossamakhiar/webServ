/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferQueuing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okhiar <okhiar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 17:16:08 by okhiar            #+#    #+#             */
/*   Updated: 2023/08/06 15:50:00 by okhiar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bufferQueuing.hpp"

bufferQueuing::bufferQueuing() : start(0), size(0), last_chunk(false)
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
	for (size_t i = 0; i < static_cast<size_t>(bytes); ++i)
		buffer_queue.push_back(buffer[i]);
}

// // ! this function will be designed later (it buggy func)
// void	bufferQueuing::extractSize(void)
// {
// 	size_t		start;
// 	int			size;
// 	std::string	s;

// 	// ! if there is no CRLF return (-1) or throw something... (handle it that the matter)
// 	while (buffer_queue.at(start) != '\r')
// 	{
// 		s += buffer_queue[start];
// 		start++;
// 	}
// 	size = Helpers::hexaToInteger(s);
// }

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
		try {
			size = extractSize(); // ? back here
		} catch (...) {
			return ;
		}
	}
	// std::cout << "\e[1;32mSize: \e[0m" << size << std::endl;
	if (size == 0){
		last_chunk = true;
		return ;
	}
	if ((buffer_queue.size() - start - 2 < size)) // ! if not possible to extract the size
		return ; // * POP
	// ! store size to use it in rec call
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
