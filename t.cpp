#include <iostream>
#include <vector>
#include <map>

void	f(int& c)
{
	c += 10;
}

void	func(int& x)
{
	f(x);
	return ;
}

int main(void)
{
	std::vector<int> v;

	for (int i = 0; i < 5; ++i)
		v[i] = 10;
	std::cout << v.size() << std::endl;
	return (0);

}