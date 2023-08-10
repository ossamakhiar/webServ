#include <iostream>
#include <vector>
#include <map>
// #include "includes/ftString.hpp"

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

// void	func()
// {
// 	ftString	str = "oussama";
// 	ftString	str1 = "oussama khiar f";

// 	str1 += "khiaraaaaa";

// 	std::cout << "size: " << str.size() << " " << str << std::endl;
// 	str.append(" khiar f");
// 	std::cout << str << std::endl;
// 	std::cout << str.substr(2, ftString::npos) << std::endl;
// 	std::cout << str.find("khiar", 9) << std::endl;
// 	std::cout << str[8] << std::endl;
// 	if (str == str1)
// 		std::cout << "Yeeaaaaah\n";
// 	if (str != str1)
// 		std::cout << "Ooaaaah\n";
// 	std::cout << str1 << std::endl;
// }

#include <iostream>
#include <sys/stat.h>

bool pathExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

int main() {
    std::string path = "./www/";

    if (pathExists(path)) {
        std::cout << "Path exists." << std::endl;
    } else {
        std::cout << "Path does not exist." << std::endl;
    }

    return 0;
}