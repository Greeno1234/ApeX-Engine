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
		//player input
	}

};

int main()
{

	//textures automatically include the "resources/" + ".png" file path
	//models automatically include the "resources/" + ".obj" file path
	std::shared_ptr<Core> core = Core::initialize();

	/////////////////////     player       //////////////////////
	std::shared_ptr<Entity> player = core->add_entity();
	player->add_component<Player>();  //has an initialise and tick function


	std::shared_ptr<Renderer> rend = player->add_component<Renderer>();
	rend->setModel(core->resources()->load<Model>("curuthers/curuthers")); //cat player

	std::shared_ptr<Audio> audio = player->add_component<Audio>();


	player->get_transform()->setScale(glm::vec3(0.3, 0.3, 0.3));



	//////////////////////     Barrel     /////////////////////////////
	std::shared_ptr<Entity> ent2 = core->add_entity();

	std::shared_ptr<TriangleRenderer> tr = ent2->add_component<TriangleRenderer>();

	tr->setTexture(core->resources()->load<Texture>("textures/brick"));  //assuming this only works withpng


	std::shared_ptr<Renderer> rend2 = ent2->add_component<Renderer>();
	rend2->setModel(core->resources()->load<Model>("Barrel/Barrel"));

	ent2->get_transform()->setPosition(glm::vec3(0.5f, -1, -3));
	ent2->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5));


	//////////////////     Floor     //////////////////////////////
	std::shared_ptr<Entity> floor = core->add_entity();
	
	floor->add_component<TriangleRenderer>();

	floor->get_transform()->setPosition(glm::vec3(0, -2, -6));
	floor->get_transform()->setScale(glm::vec3(5, 5, 2));
	floor->get_transform()->setRotation(-90, glm::vec3(1,0,0));
	
	//ent2->kill();
	
	
	//core->resources()->load<Texture>("");
	
	
	core->start();
	audio->on_initialise();
	return(0);
}