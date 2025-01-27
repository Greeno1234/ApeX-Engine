#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "AudioSource.h"
#include "Core.h"
#include "Resources.h"
#include "Entity.h"
#include "Audio.h"

//#include <stdexcept>
#include <vector>
#include <string>

namespace apex {

    void AudioSource::setAudio(std::shared_ptr<Audio> audio)
    {
        m_audio = audio;
    }


    void AudioSource::Play()//
    {
        // preparing sound source
        alGenSources(1, &sourceId);

        alSourcei(sourceId, AL_BUFFER, m_audio->bufferId);
        alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f,0.0f); // position of the sound's source
        alSourcef(sourceId, AL_PITCH, 1); // raises or lowers pitch
        //alSourcef(sourceId, AL_GAIN, vol); // raises or lowers volume

        alSourcePlay(sourceId); ///< Play audio
        std::cout << "audio playing";
        //setLoop(true);
    }

    void AudioSource::setLoop(bool go)
    {

        while (go)
        {

            int state = 0;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) break;
            alSourcePlay(sourceId);
            std::cout << "audio playing";
            //alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

    #ifdef _WIN32
        Sleep(10);
    #else
        usleep(100 * 1000);
    #endif
        }

    }


    AudioSource::~AudioSource()
    {
    // cleanup
    alDeleteSources(1, &sourceId);
    alDeleteBuffers(1, &m_audio->bufferId); 
    }

}