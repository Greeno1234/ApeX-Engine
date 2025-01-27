#include <iostream>
#include <ApeX/apex.h>


using namespace apex;

std::shared_ptr<Core> core = Core::initialize(); ///< initializes Core


struct Player : Component
{
	int tick = SDL_GetTicks();
	int lastTick = tick;
	glm::vec3 pos;
	float angle = -90;
	float pitch = 0;
	//float speed;
	//        = 
	float distance = 0.05f;
	//        /
	//float time;
	bool firstPersonMode = true;
	bool delay = false;
	
	void on_initialize()
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
	

		glm::vec3 direction = camera->getDirection();
		glm::vec3 dirRight = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));


		switch (keyboard->whichKey()) //< checks which keys exist in the "m_keys" int vector
		{
		case (SDLK_w):
			
			pos += distance * direction;
			break;

		case (SDLK_a):
			//pos.x -= distance;
			pos -= distance * dirRight;
			break;

		case (SDLK_s):
			pos -= distance * direction;
			break;

		case (SDLK_d):
			pos += distance * dirRight;
			break;

		case (SDLK_LEFT):
			angle -= 2;
			break;

		case (SDLK_RIGHT):
			angle += 2;
			break;
		case (SDLK_UP):
			pitch -= 2;
			break;
		case (SDLK_DOWN):
			pitch += 2;
			break;

		case (SDLK_SPACE):

			if (delay == false) ///< checks if a delay is activated
			{
				as->Play(); 
				delay = true;///< set a delay before allowing interaction
			}
			break;
		case (SDLK_p):
			if (delay == false)
			{
				if (firstPersonMode == true)
				{
					firstPersonMode = false;
				}
				else
				{
					firstPersonMode = true;
				}
				delay = true;
			}
			
			
			break;
		default:
			break;
		}

		pos.y = -1.25; //keeps player on ground
		entity()->get_transform()->setRotation(-angle +90, glm::vec3(0, 1, 0));
		entity()->get_transform()->setPosition(pos);


		// Camera
		if (firstPersonMode == true)
		{
			camera->setPosition(pos + glm::vec3(0, 1, 0));// First person camera
			// Constrains the pitch to stop it from repeating in circles
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
			camera->setPitch(-pitch);
		}
		else // third person
		{
			camera->setPosition(pos + (glm::vec3(2, 4, 2) * -direction));// Third person camera
			if (pitch > -20.0f)
				pitch = -20.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
			camera->setPitch(pitch);
		}
		//camera->setTarget(pos);
		camera->setYaw(angle);
		
		tick = SDL_GetTicks();
		if (tick - lastTick >= 300)
		{
			lastTick = tick;
			//std::cout << tick - lastTick << std::endl;
			std::cout << "tick" << std::endl;
			delay = false; //reset the delay for button presses
		}
	}

};

int main()
{
	 
	//textures automatically include the "resources/" + ".png" file path
	//models automatically include the "resources/" + ".obj" file path


	//Models
	std::shared_ptr<Model> mCat = core->resources()->load<Model>("curuthers/curuthers");
	std::shared_ptr<Model> mBarrel = core->resources()->load<Model>("Barrel/Barrel");
	std::shared_ptr<Model> mCar = core->resources()->load<Model>("Car/l200");

	//Textures
	std::shared_ptr<Texture> tBrick = core->resources()->load<Texture>("textures/brick");
	std::shared_ptr<Texture> tControls = core->resources()->load<Texture>("textures/controls");

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
	
	std::shared_ptr<BoxCollider> bc1 = player->add_component<BoxCollider>();
	bc1->setSize(glm::vec3(1, 1, 1));
	player->add_component<RigidBody>();



	/////////////////////////////////////     Barrels     //////////////////////////////////////////
	std::shared_ptr<Entity> barrel = core->add_entity();
	std::shared_ptr<Entity> barrel2 = core->add_entity();
	
	std::shared_ptr<Renderer> barrelModel = barrel->add_component<Renderer>();
	barrelModel->setModel(mBarrel);

	barrel->get_transform()->setPosition(glm::vec3(0.5f, -2, -6));
	barrel->get_transform()->setScale(glm::vec3(0.5, 0.5, 0.5));

	std::shared_ptr<Renderer> barrelModel2 = barrel2->add_component<Renderer>();
	barrelModel2->setModel(mBarrel);
	barrel2->get_transform()->setPosition(glm::vec3(0.5f, -2, -10));


	//////////////////////////////     Car       /////////////////////////////////

	std::shared_ptr<Entity> car = core->add_entity();
	std::shared_ptr<Renderer> carModel = car->add_component<Renderer>();
	carModel->setModel(mCar);

	car->get_transform()->setPosition(glm::vec3(0, -2, -20));
	car->get_transform()->setScale(glm::vec3(3, 3, 3));
	car->get_transform()->setRotation(75, glm::vec3(0, 1, 0));

	///////////////////////Triangle////////////////////////////////////
	std::shared_ptr<Entity> controls = core->add_entity();
	std::shared_ptr<TriangleRenderer> tr = controls->add_component<TriangleRenderer>();  //triangle rendererr now renders squares instead of traingles

	tr->setTexture(tControls);
	controls->get_transform()->setScale(glm::vec3(3, 3, 1));
	controls->get_transform()->setPosition(glm::vec3(0, 2, -12));
	

	///////////////////////wall////////////////////////////////////
	std::shared_ptr<Entity> wall = core->add_entity();
	std::shared_ptr<TriangleRenderer> trw = wall->add_component<TriangleRenderer>();

	trw->setTexture(tBrick);
	wall->get_transform()->setScale(glm::vec3(10, 5, 1));
	wall->get_transform()->setPosition(glm::vec3(4, -1, -7));
	wall->get_transform()->setRotation(90 ,glm::vec3(0,1,0));

	///////////////////////wall2////////////////////////////////////
	std::shared_ptr<Entity> wall2 = core->add_entity();
	std::shared_ptr<TriangleRenderer> trw2 = wall2->add_component<TriangleRenderer>();  

	trw2->setTexture(tBrick);
	wall2->get_transform()->setScale(glm::vec3(10, 5, 1));
	wall2->get_transform()->setPosition(glm::vec3(-4, -1, -7));
	wall2->get_transform()->setRotation(90, glm::vec3(0, 1, 0));

	///////////////////////wall3////////////////////////////////////
	std::shared_ptr<Entity> wall3 = core->add_entity();
	std::shared_ptr<TriangleRenderer> trw3 = wall3->add_component<TriangleRenderer>(); 

	trw3->setTexture(tBrick);
	wall3->get_transform()->setScale(glm::vec3(10, 5, 1));
	wall3->get_transform()->setPosition(glm::vec3(4, -1, -7));
	wall3->get_transform()->setRotation(90, glm::vec3(0, 1, 0));


	std::shared_ptr<BoxCollider> bc2 = wall->add_component<BoxCollider>();
	bc2->setSize(glm::vec3(1,1,1));
	wall->add_component<RigidBody>();




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