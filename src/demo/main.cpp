#include <iostream>
#include <ApeX/apex.h>

using namespace apex;

struct Player : Component
{
	//int m_dummy;

	void on_initialize() //virtual functions?? within component
	{
		printf("Player::initialize\n");
	}

	void on_tick()
	{
		printf("Player::tick\n");
	}


};

int main()
{
	std::shared_ptr<Core> core = Core::initialize();

	std::shared_ptr<Entity> ent = core->add_entity();
	ent->add_component<Player>();


<<<<<<< Updated upstream
=======
	ent->add_component<TriangleRenderer>();
	ent->add_component<TriSpin>();

>>>>>>> Stashed changes

	core->start();


	//std::cout << "Hello World" << std::endl;

	return(0);
}