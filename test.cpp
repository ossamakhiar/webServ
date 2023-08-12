#include <iostream>
#include <vector>
#include <map>
// #include <cstdlib>
#include "ftString.hpp"
#include "helpers.hpp"
#include <fstream>
#include <stdexcept>

std::vector<std::string>	split(std::string str, const std::string& charset)
{
	std::vector<std::string>	tokens;
	std::string					token;
	size_t						pos;

	while ((pos = str.find_first_of(charset)) != std::string::npos)
	{
		token = str.substr(0, pos);
		str.erase(0, pos + 1);
		if (!token.empty())
			tokens.push_back(token);
	}
	if (!str.empty())
		tokens.push_back(str);
	return (tokens);
}


class Test
{
	std::string x;
public:
	void	setX(const std::string&);
};

typedef void (Test::*mfunc)(const std::string&);

class Second
{
	std::map<std::string, mfunc>	m;
public:
	Second()
	{
		m.insert(std::pair<std::string, mfunc>("oussama", &Test::setX));
	}
	void doWork();
};

void	Test::setX(const std::string& b)
{
	x = b;
	std::cout << x << std::endl;
}

void	Second::doWork()
{
	Test	t;

	(t.*(m["oussama"]))("khiar");
}

int			safeAtoi(const std::string& str)
{
	char	*eptr;
	long	res;

	res = std::strtol(str.c_str(), &eptr, 10);
	if (*eptr != '\0')
		throw std::runtime_error("bad argument");
	std::cout << res << "*" << INT_MAX << std::endl;
	if (res > INT_MAX || res < INT_MIN)
		throw std::runtime_error("can't convert");
	return (res);
}


int	main(void)
{
	// Second	s;

	// s.doWork();
	// try {
	// 	int res = safeAtoi("5885858585856985585858");
	// 	std::cout << res << std::endl;
	// } catch (std::exception& e)
	// {
	// 	std::cout << e.what() << std::endl;
	// }
	// std::cout << (1 << 16) << std::endl;
	// unsigned short x = -1;
	// std::cout << x << " " << (1 << 16) - 1 << std::endl;
	ftString	str;
	std::string	x = "oussama khiar";
	str = x;
	// std::ifstream	in;

	// in.open("t.c");
	// Helpers::readFile(in, str);
	std::cout << str << std::endl;
	return (0);
}





	// std::string str = "H";
    // std::string delimiter = ", !?";
	// std::vector<std::string>	tokens;

	// tokens = split(str, delimiter);
	// for (int i = 0; i < tokens.size(); i++)
	// 	std::cout << tokens[i] << " " << i << std::endl;
	// std::vector<int>	v;
	// int		x;

	// x = 10;
	// v.push_back(x);
	// std::string buffer = "oussama khiar {";
	// size_t pos = buffer.find_first_of(" ");
	// std::cout << buffer.substr(0, pos);
	// std::cout << "\n" << pos;