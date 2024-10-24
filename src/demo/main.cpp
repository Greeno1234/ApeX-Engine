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

#undef main

int main()
{
	std::shared_ptr<Core> core = Core::initialize();

	std::shared_ptr<Entity> ent = core->add_entity();
	ent->add_component<Player>();


	ent->add_component<TriangleRenderer>();


	core->start();

	

	std::cout << "test";

	//std::cout << "Hello World" << std::endl;

	return(0);
}