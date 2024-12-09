#include <iostream>
#include <ApeX/apex.h>

using namespace apex;

struct Player : Component
{
	//std::shared_ptr<TriangleRenderer> tr;

	void on_initialize() //virtual functions?? within component
	{
		printf("Player::initialize\n");
		//tr = entity()->get_component<TriangleRenderer>();
	}
	int i = 0;
	void on_tick()
	{

		//implement delta time and only show tick for frame of 60


		//printf("Player::tick\n");
		
	}

};

int main()
{
	std::shared_ptr<Core> core = Core::initialize();

	std::shared_ptr<Entity> ent = core->add_entity();

	std::shared_ptr<Entity> ent2 = core->add_entity();
	

	ent->add_component<Player>();  //has an initialise and tick function
	ent->add_component<TriangleRenderer>();




	ent2->add_component<TriangleRenderer>();

	// Transform
	ent->get_transform()->setPosition(glm::vec3(0.5, 0, -2)); // sets position of entity
	ent->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5)); // sets scale to half


	ent2->get_transform()->setPosition(glm::vec3(0, 0, -2));
	
	//ent2->kill();
	//core->resources()->load<Model>("models/curuthers/curuthers");
	//core->resources()->load<Texture>("");

	core->start();

	return(0);
}