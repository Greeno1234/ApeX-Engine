#include <iostream>
#include <ApeX/apex.h>

using namespace apex;

struct Test   //::Component
{
	int m_temp;
};

int main()
{
	std::shared_ptr<Core> core = Core::initialize();

	std::shared_ptr<Entity> ent = core->add_entity();
	ent->add_component<Test>();



	core->start();


	//std::cout << "Hello World" << std::endl;

	return(0);
}