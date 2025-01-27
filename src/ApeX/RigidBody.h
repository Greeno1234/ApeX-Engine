#include "Component.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace apex
{
	struct BoxCollider;

	struct RigidBody : Component
	{
		std::vector<std::shared_ptr<BoxCollider>> colliders;
		void on_tick();

	};
}