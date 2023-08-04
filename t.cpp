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

	int	x = 15;
	func(x);
	std::cout << x << std::endl;
	return (0);

}