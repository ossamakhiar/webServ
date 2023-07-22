#include <iostream>
#include <vector>

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

int	main(void)
{
	std::string str = "H";
    std::string delimiter = ", !?";
	std::vector<std::string>	tokens;

	tokens = split(str, delimiter);
	for (int i = 0; i < tokens.size(); i++)
		std::cout << tokens[i] << " " << i << std::endl;
	return (0);
}