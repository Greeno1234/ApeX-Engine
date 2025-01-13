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
	std::shared_ptr<Core> core = Core::initialize(); ///< initializes Core (which does.......)
	//textures automatically include the "resources/" + ".png" file path
	//models automatically include the "resources/" + ".obj" file path



	///////////////////////////////////     Player       //////////////////////////////////
	std::shared_ptr<Entity> player = core->add_entity(); ///< adds a player entity to core
	player->add_component<Player>();  //has an initialise and tick function


	std::shared_ptr<Renderer> rend = player->add_component<Renderer>();
	rend->setModel(core->resources()->load<Model>("curuthers/curuthers")); //cat player

	std::shared_ptr<Audio> audio = player->add_component<Audio>();
	audio->on_initialise();
	audio->Play();

	player->get_transform()->setScale(glm::vec3(0.3, 0.3, 0.3));
	player->get_transform()->setPosition(glm::vec3(0, -1.25, -3));


	
	/////////////////////////////////////     Barrels     //////////////////////////////////////////
	std::shared_ptr<Entity> barrel = core->add_entity();
	std::shared_ptr<Entity> barrel2 = core->add_entity();
	//std::shared_ptr<TriangleRenderer> tr = barrel->add_component<TriangleRenderer>();

	//tr->setTexture(core->resources()->load<Texture>("textures/brick"));  //assuming this only works withpng
	std::shared_ptr<Renderer> barrelModel = barrel->add_component<Renderer>(); // can one renderer be used for multiple things or do i have to add a renderer to each entity but save the model????
	barrelModel->setModel(core->resources()->load<Model>("Barrel/Barrel"));

	barrel->get_transform()->setPosition(glm::vec3(0.5f, -2, -6));
	barrel->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5));

	std::shared_ptr<Renderer> barrelModel2 = barrel2->add_component<Renderer>();
	barrelModel2->setModel(core->resources()->load<Model>("Barrel/Barrel"));
	barrel2->get_transform()->setPosition(glm::vec3(0.5f, -2, -10));






	/////////////////////////////////////     Floor     /////////////////////////////////////
	std::shared_ptr<Entity> floor = core->add_entity();
	
	floor->add_component<TriangleRenderer>();

	floor->get_transform()->setPosition(glm::vec3(0, -2, -6));
	floor->get_transform()->setScale(glm::vec3(5, 5, 2));
	floor->get_transform()->setRotation(-90, glm::vec3(1,0,0));
	
	//ent2->kill();
	
	
	//core->resources()->load<Texture>("");
	
	
	core->start();
	
	return(0);
}