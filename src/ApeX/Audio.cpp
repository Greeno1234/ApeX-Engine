#include "Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdexcept>
#include <vector>
#include <string>

#include "stb_vorbis.c"



namespace apex {


void load_ogg(const std::string&, std::vector<unsigned char>&, ALenum&, ALsizei&); /// initialises the thing with default things

int Audio::on_initialise()
{
    std::cout << "working";
    /*************************************************************************
     * Initialization
     *************************************************************************/
    ALCdevice* device = alcOpenDevice(NULL); //ALC ?? audio library???

    if (!device)
    {
        throw std::runtime_error("Failed to open audio device");
    }

    ALCcontext* context = alcCreateContext(device, NULL);

    if (!context)
    {
        alcCloseDevice(device);
        throw std::runtime_error("Failed to create audio context");
    }

    if (!alcMakeContextCurrent(context))
    {
        alcDestroyContext(context);
        alcCloseDevice(device);
        throw std::runtime_error("Failed to make context current");
    }

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    //alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);







    /*************************************************************************
     * Preparing buffer
     *************************************************************************/
    ALenum format = 0;
    ALsizei freq = 0;
    std::vector<unsigned char> bufferData;
    //load_ogg("../dixie_horn.ogg", bufferData, format, freq);
    load_ogg("../resources/fnaf_honk.ogg", bufferData, format, freq);

    ALuint bufferId = 0;
    alGenBuffers(1, &bufferId);

    alBufferData(bufferId, format, &bufferData.at(0),
        static_cast<ALsizei>(bufferData.size()), freq);







    /*************************************************************************
     * Preparing sound source
     *************************************************************************/
    ALuint sourceId = 0;////
    alGenSources(1, &sourceId);

    alSourcei(sourceId, AL_BUFFER, bufferId);
    //alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
    //alSourcef(sourceId, AL_PITCH, 10);
    //alSourcef(sourceId, AL_GAIN, vol);






    /*************************************************************************
     * Play audio
     *************************************************************************/
    alSourcePlay(sourceId);

    //Play();






    /*************************************************************************
     * Tick
     *************************************************************************/

    while (true)
    {
        int state = 0;
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) break;

        //alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

#ifdef _WIN32
        Sleep(10);
#else
        usleep(100 * 1000);
#endif
    }







    /*************************************************************************
     * Cleanup
     *************************************************************************/
    alDeleteSources(1, &sourceId);
    alDeleteBuffers(1, &bufferId);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}


void Audio::Play()//
{

    /*************************************************************************
     * Play audio
     *************************************************************************/

    //alSourcePlay(sourceId);
}



void load_ogg(const std::string& _path, std::vector<unsigned char>& _buffer,
    ALenum& _format, ALsizei& _freq)
{
    int channels = 0;
    int sampleRate = 0;
    short* output = NULL;

    size_t samples = stb_vorbis_decode_filename(_path.c_str(),
        &channels, &sampleRate, &output);

    if (samples == -1)
    {
        throw std::runtime_error("Failed to open file '" + _path + "' for decoding");
    }

    // Record the format required by OpenAL
    if (channels < 2)
    {
        _format = AL_FORMAT_MONO16;
    }
    else
    {
        _format = AL_FORMAT_STEREO16;
    }

    // Copy (# samples) * (1 or 2 channels) * (16 bits == 2 bytes == short)
    _buffer.resize(samples * channels * sizeof(short));
    memcpy(&_buffer.at(0), output, _buffer.size());

    // Record the sample rate required by OpenAL
    _freq = sampleRate;

    // Clean up the read data
    free(output);
}

}