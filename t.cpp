#include <iostream>
#include <map>

class Entity
{
	private:
		std::map<std::string, int> m;
	public:
		Entity();
		void func();
};

Entity::Entity()
{
ç
}

void Entity::func()
{
	for (std::map<std::string, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}
}

int main(void)
{
	Entity e;

	e.func();

}