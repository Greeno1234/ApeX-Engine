#include "Resource.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace apex
{
	struct AudioSource;
	struct Resource;

	struct Audio : Resource
	{

		void onLoad();

	private:
		friend struct apex::AudioSource;

		ALuint bufferId = 0;

	};

}