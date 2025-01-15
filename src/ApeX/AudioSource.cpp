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


/*************************************************************************
 * Preparing sound source
 *************************************************************************/

    alGenSources(1, &sourceId);

    alSourcei(sourceId, AL_BUFFER, m_audio->bufferId);
    alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f); // replace with models location
    alSourcef(sourceId, AL_PITCH, 1); // lowers pitch and makes it sound like a cat lol
    //alSourcef(sourceId, AL_GAIN, vol); // raises or lowers volume

    /*************************************************************************
     * Play audio
     *************************************************************************/
    alSourcePlay(sourceId);
    std::cout << "audio playing";
    //setLoop(true);

}

void AudioSource::setLoop(bool go)
{

    /*************************************************************************
     * Tick
     *************************************************************************/


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
    /*************************************************************************
* Cleanup
*************************************************************************/
alDeleteSources(1, &sourceId);
alDeleteBuffers(1, &m_audio->bufferId); 
}

}