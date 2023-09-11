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



// void	bubbleSort(std::vector<int>& nums)
// {
// 	size_t	i, j;
// 	int	tmp;

// 	i = 0;
// 	while (i < nums.size() - 1)
// 	{
// 		j = 0;
// 		while (j < nums.size() - i - 1)
// 		{
// 			if (nums[j] > nums[j + 1])
// 			{
// 				// swapping
// 				tmp = nums[j];
// 				nums[j] = nums[j + 1];
// 				nums[j + 1] = tmp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// int	main(void)
// {
// 	std::vector<int>	nums;

// 	nums.push_back(12);
// 	nums.push_back(10);
// 	nums.push_back(1);
// 	nums.push_back(2);
// 	nums.push_back(-1);
// 	nums.push_back(3);
// 	nums.push_back(5);

// 	for (size_t	i = 0; i < nums.size(); ++i)
// 		std::cout << "tab[" << i << "]: " << nums[i] << " ";
// 	std::cout << std::endl;
// 	bubbleSort(nums);
// 	for (size_t	i = 0; i < nums.size(); ++i)
// 		std::cout << "tab[" << i << "]: " << nums[i] << " ";
// 	std::cout << std::endl;
// 	return (0);
// }


/*
5 1 2 0 : 0, 0
	1 5 2 0: 0, 1
	1 2 5 0: 0, 2
	1 2 0 5: 0, 3

1 2 0 5: 
*/ 