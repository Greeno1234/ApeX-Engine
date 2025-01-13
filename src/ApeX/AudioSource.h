#include <AL/al.h>
#include <AL/alc.h>

#include "Component.h"
namespace apex
{
	struct Audio;

	struct AudioSource : Component
	{
		AudioSource::~AudioSource();
		void Play();

		void setAudio(std::shared_ptr<Audio> audio);

		void setLoop(bool go);



	private:
	
		std::shared_ptr<Audio> m_audio;
		ALuint sourceId = 0;
		//ALuint sourceID;

	};

}