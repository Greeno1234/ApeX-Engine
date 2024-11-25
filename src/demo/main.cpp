#include <iostream>
#include <ApeX/apex.h>

using namespace apex;

struct Player : Component
{

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
	//add a transform to this
	ent->add_component<Player>();  //find out what this does
	ent->add_component<TriangleRenderer>();
	//ent->add_component<TriSpin>();
	// 
	//core->resources()->load<Model>("models/curuthers/curuthers");
	//core->resources()->load<Texture>("");

	core->start();

	return(0);
}