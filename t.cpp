// #include <stdio.h>
// #include <poll.h>

// int main(void)
// {
// 	struct pollfd pfds[1]; // More if you want to monitor more

// 	pfds[0].fd = 0; // Standard input
// 	pfds[0].events = POLLIN; // Tell me when ready to read
// 	// If you needed to monitor other things, as well:
// 	//pfds[1].fd = some_socket; // Some socket descriptor
// 	//pfds[1].events = POLLIN; // Tell me when ready to read
// 	printf("Hit RETURN or wait 2.5 seconds for timeout\n");
// 	int num_events = poll(pfds, 1, 2500); // 2.5 second timeout
// 	if (num_events == 0) {
// 		printf("Poll timed out!\n");
// 	} else {
// 		int pollin_happened = pfds[0].revents & POLLIN;
// 		if (pollin_happened) {
// 			printf("File descriptor %d is ready to read\n", pfds[0].fd);
// 		} else {
// 			printf("Unexpected event occurred: %d\n", pfds[0].revents);
// 		}
// 	}
// 	return 0;
// }

// #include <iostream>

// int	main(void)
// {
// 	char x = 10;
// 	char *p = static_cast<char *>(x);

// 	std::cout << (void *)p << std::endl;
// 	return (0);
// }
#include <iostream>
#include <vector>
#include <map>

int main(void)
{
	// int	x = 15;
	// int y = 10;
	// std::vector<int> vec;
	// vec.push_back(x);
	// std::vector<int>::iterator it = vec.begin();
	// int	*ptr = &(*it);
	// *ptr = 1500;
	// std::cout << vec[0] << std::endl;
	// std::map<std::string, int> m;
	// m.insert(std::pair<std::string, int>("oussama", 15));

	// try {
	// 	std::cout << m.at("d") << std::endl;
	// } catch (std::exception& e) {
	// 	std::cout << e.what() << std::endl;
	// }

	// std::map<std::string, int>::iterator itt = m.begin();
	// if (itt == m.end())
	// 	std::cout << "empty container\n";

	// std::map<std::string, int>::iterator it = m.lower_bound("key");
	// std::cout << it->first << " " << it->second << std::endl;
	// std::cout << m["500"] << std::endl;
	// std::pair<std::string, int>	p("key", 8);
	// std::pair<std::string, int>	pp("key", 8);

	// if (p == pp)
	// 	std::cout << "Equal\n";


	std::string oussama;

	oussama.append(1, '.');
	oussama.append(1, '2');
	std::cout << oussama << std::endl;

	return (0);

}