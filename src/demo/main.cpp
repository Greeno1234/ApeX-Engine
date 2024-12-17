#include <iostream>
#include <ApeX/apex.h>

using namespace apex;

struct Player : Component
{
	

	void on_initialize() //virtual functions?? within component
	{
		printf("Player::initialize\n");	
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




	std::shared_ptr<TriangleRenderer> tr = ent2->add_component<TriangleRenderer>();
	tr->setTexture(core->resources()->load<Texture>("textures/brick.png"));  //assuming this only works withpng
	
	//core->resources()->load<Texture>("../resources/textures/grass.png");

	// Transform
	ent->get_transform()->setPosition(glm::vec3(0.5, 0, -2)); // sets position of entity
	ent->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5)); // sets scale to half

	
	ent2->get_transform()->setPosition(glm::vec3(0.5f, 0, -2));
	ent2->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5));


	
	//ent2->kill();
	//core->resources()->load<Model>("models/curuthers/curuthers");
	//core->resources()->load<Texture>("");

	core->start();

	return(0);
}