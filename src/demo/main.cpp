#include <iostream>
#include <ApeX/apex.h>


using namespace apex;

std::shared_ptr<Core> core = Core::initialize(); ///<initialise core (receently moved here(maybe needs to move back to main but idk))


struct Player : Component
{
	
	glm::vec3 pos;
	float angle = 180;
	//float speed;
	//        = 
	float distance = 0.025f;
	//        /
	//float time;

	void on_initialize() //virtual functions?? within component
	{
		printf("Player::initialize\n");	
		
		pos = glm::vec3(0, -1.25, -3); ///< Starting postition of player
	}
	void on_tick()
	{
		std::shared_ptr<Keyboard> keyboard = entity()->core()->getKeyboard();
		std::shared_ptr<Camera> camera = entity()->core()->getCamera();

		std::shared_ptr<AudioSource> as = entity()->get_component<AudioSource>();
		//implement delta time and only show tick for frame of 60
		
		//try switch statement


		switch (keyboard->whichKey()) //< checks which keys exist in the "m_keys" int vector
		{
		case (SDLK_w):
			pos.z -= distance;
				break;

		case (SDLK_a):
			pos.x -= distance;
			break;

		case (SDLK_s):
			pos.z += distance;
			break;

		case (SDLK_d):
			pos.x += distance;
			break;

		case (SDLK_LEFT):
			angle -= 5;
			break;

		case (SDLK_RIGHT):
			angle += 5;
			break;

		case (SDLK_SPACE):
			as->Play();
			break;

		default:
			break;
		}

		entity()->get_transform()->setRotation(angle, glm::vec3(0, 1, 0));
		entity()->get_transform()->setPosition(pos);

	    camera->setPosition(glm::vec3 (pos.x,pos.y + 1,pos.z + 3));//set to pos
		
	}

};

int main()
{
	 ///< initializes Core (which does.......)
	//textures automatically include the "resources/" + ".png" file path
	//models automatically include the "resources/" + ".obj" file path


	//Models
	std::shared_ptr<Model> mCat = core->resources()->load<Model>("curuthers/curuthers");
	std::shared_ptr<Model> mBarrel = core->resources()->load<Model>("Barrel/Barrel");

	//Textures
	std::shared_ptr<Texture> tBrick = core->resources()->load<Texture>("textures/brick");

	//Audio

	std::shared_ptr<Audio> aHonk = core->resources()->load<Audio>("fnaf_honk");
	std::shared_ptr<Audio> aHorn = core->resources()->load<Audio>("dixie_horn");


///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////     Player       ////////////////////////////////////////////
	std::shared_ptr<Entity> player = core->add_entity(); ///< adds a player entity to core
	


	std::shared_ptr<AudioSource> as = player->add_component<AudioSource>();
	as->setAudio(aHonk); ///< Set audio to honk

	std::shared_ptr<Renderer> rend = player->add_component<Renderer>();
	rend->setModel(mCat); //cat player

	player->get_transform()->setScale(glm::vec3(0.3, 0.3, 0.3));
	player->get_transform()->setPosition(glm::vec3(0, -1.25, -3));

	player->add_component<Player>();  //has an initialise and tick function
	




	/////////////////////////////////////     Barrels     //////////////////////////////////////////
	std::shared_ptr<Entity> barrel = core->add_entity();
	std::shared_ptr<Entity> barrel2 = core->add_entity();
	
	std::shared_ptr<Renderer> barrelModel = barrel->add_component<Renderer>(); // can one renderer be used for multiple things or do i have to add a renderer to each entity but save the model????
	barrelModel->setModel(mBarrel);

	barrel->get_transform()->setPosition(glm::vec3(0.5f, -2, -6));
	barrel->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5));

	std::shared_ptr<Renderer> barrelModel2 = barrel2->add_component<Renderer>();
	barrelModel2->setModel(mBarrel);
	barrel2->get_transform()->setPosition(glm::vec3(0.5f, -2, -10));




	///////////////////////Triangle////////////////////////////////////
	std::shared_ptr<Entity> triangle = core->add_entity();
	std::shared_ptr<TriangleRenderer> tr = triangle->add_component<TriangleRenderer>();
	tr->setTexture(tBrick);


	triangle->get_transform()->setPosition(glm::vec3(0, -1, -20));
	triangle->get_transform()->setScale(glm::vec3(5, 5, 5));





	/////////////////////////////////////     Floor     /////////////////////////////////////
	std::shared_ptr<Entity> floor = core->add_entity();
	
	floor->add_component<TriangleRenderer>();

	floor->get_transform()->setPosition(glm::vec3(0, -2, -7));
	floor->get_transform()->setScale(glm::vec3(50, 50, 50));
	floor->get_transform()->setRotation(-90, glm::vec3(1,0,0));
	/////////////////////////////////////////////////////////////////////////////////////
	




	core->start();
	
	return(0);
}