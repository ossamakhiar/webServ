#include <iostream>
#include <vector>
#include <map>

int	getDigit(const char& c)
{
	std::cout << c << std::endl;
	if (c <= 57 && c >= 48)
		return (c - 48);
	if (c < 97 || c > 102)
		throw std::runtime_error("hexa-to-int: bad input");
	return (c - 87);
}

unsigned int	hexaToInteger(const std::string& hex)
{
	unsigned int	result;
	int				digit;
	size_t			i;
	char			c;

	i = result = 0;
	while (i < hex.length())
	{
		c = hex[i];
		if (c >= 65 && c <= 90)
			c += 32;
		digit = getDigit(c);
		result *= 16; // * haxadecimal Base;
		result += digit;
		i++;
	}
	return (result);
}

std::string	randomFileNameGen(void)
{
	std::string	alphanums = "_";
	std::string	file_name;

	std::srand(time(NULL));
	for (size_t i = 0; i < 26; ++i)
	{
		if (i <= 9)
			alphanums += (i + 48);
		alphanums += (i + 97);
		alphanums += (i + 65);
	}
	// ! BASE = 26 + 26 + 10 + 1 = 63
	for (size_t i = 0; i < 15; i++)
		file_name += alphanums[rand() % 63];
	return (file_name);
}

int main(void)
{
	// std::cout << hexaToInteger("10") << std::endl;
	std::cout << randomFileNameGen() << std::endl;
	return (0);

}