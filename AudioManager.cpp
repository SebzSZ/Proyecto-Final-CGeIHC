#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION

#include <cstdio>
#include <glm.hpp>

#include "dr_wav.h"
#include "dr_mp3.h"

#include "AudioManager.h"

// Inicialización
bool AudioManager::Initialize()
{
    // Abrir el dispositivo de audio predeterminado del sistema
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        printf("[Audio] Error: No se pudo abrir el dispositivo de audio.\n");
        return false;
    }

    // Crear el contexto de OpenAL
    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        printf("[Audio] Error: No se pudo crear el contexto de audio.\n");
        alcCloseDevice(device);
        device = nullptr;
        return false;
    }

    alcMakeContextCurrent(context);

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    ALfloat orientation[] = { 0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f };
    alListenerfv(AL_ORIENTATION, orientation);

    return true;
}

// Carga de sonidos
bool AudioManager::loadWAV(const std::string& name, const std::string& filePath)
{
    if (clips.count(name))
    {
        printf("[Audio] Aviso: '%s' ya esta cargado, se omite.\n", name.c_str());
        return true;
    }

    drwav wav;
    if (!drwav_init_file(&wav, filePath.c_str(), nullptr))
    {
        printf("[Audio] Error: No se pudo abrir '%s'.\n", filePath.c_str());
        return false;
    }

    drwav_uint64 totalFrames = wav.totalPCMFrameCount;
    drwav_uint16 channels = wav.channels;
    drwav_uint32 sampleRate = wav.sampleRate;

    std::vector<drwav_int16> samples(totalFrames * channels);
    drwav_read_pcm_frames_s16(&wav, totalFrames, samples.data());
    drwav_uninit(&wav);

    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, samples.data(), (ALsizei)(samples.size() * sizeof(drwav_int16)), (ALsizei)sampleRate);

    checkError("loadWAV: alBufferData");

    ALuint source = createSource(buffer, false, 1.0f);

    clips[name] = { buffer, source };
    return true;
}

bool AudioManager::loadMP3(const std::string& name, const std::string& filePath)
{
    if (clips.count(name))
    {
        printf("[Audio] Aviso: '%s' ya esta cargado, se omite.\n", name.c_str());
        return true;
    }

    drmp3_config config;
    drmp3_uint64 totalFrames;

    drmp3_int16* samples = drmp3_open_file_and_read_pcm_frames_s16(filePath.c_str(), &config, &totalFrames, nullptr);

    if (!samples)
    {
        printf("[Audio] Error: No se pudo abrir '%s'.\n", filePath.c_str());
        return false;
    }

    ALenum format = (config.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, samples, (ALsizei)(totalFrames * config.channels * sizeof(drmp3_int16)), (ALsizei)config.sampleRate);

    drmp3_free(samples, nullptr);
    checkError("loadMP3: alBufferData");

    ALuint source = createSource(buffer, false, 1.0f);
    clips[name] = { buffer, source };

    return true;
}

// Reproducción y control
void AudioManager::play(const std::string& name, bool loop, float gain)
{
    auto it = clips.find(name);
    if (it == clips.end())
    {
        printf("[Audio] Error: '%s' no esta cargado.\n", name.c_str());
        return;
    }

    AudioClip& clip = it->second;

    // Reconfigurar loop y gain antes de reproducir
    alSourcei(clip.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcef(clip.source, AL_GAIN, gain);

    // Regresar al inicio si ya habia terminado
    alSourceRewind(clip.source);
    alSourcePlay(clip.source);
    checkError("play");
}

void AudioManager::stop(const std::string& name)
{
    auto it = clips.find(name);
    if (it != clips.end())
        alSourceStop(it->second.source);
}

void AudioManager::pause(const std::string& name)
{
    auto it = clips.find(name);
    if (it != clips.end())
        alSourcePause(it->second.source);
}

void AudioManager::resume(const std::string& name)
{
    auto it = clips.find(name);
    if (it != clips.end())
        alSourcePlay(it->second.source);
}

void AudioManager::setGain(const std::string& name, float gain)
{
    auto it = clips.find(name);
    if (it != clips.end())
        alSourcef(it->second.source, AL_GAIN, gain);
}

bool AudioManager::isPlaying(const std::string& name) const
{
    auto it = clips.find(name);
    if (it == clips.end()) return false;

    ALint state;
    alGetSourcei(it->second.source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

// Fade in/out
void AudioManager::fadeIn(const std::string& name, float duration, bool loop, bool resume)
{
    auto it = clips.find(name);
    if (it == clips.end())
    {
        printf("[Audio] Error: '%s' no esta cargado.\n", name.c_str());
        return;
    }

    // Si no es resume, iniciar desde el principio
    if (!resume)
    {
        alSourceRewind(it->second.source);
        alSourcei(it->second.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(it->second.source, AL_GAIN, 0.0f);
        alSourcePlay(it->second.source);
    }
    else
    {
        // Si es resume, reanudar desde donde estaba pausado
        alSourcei(it->second.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(it->second.source, AL_GAIN, 0.0f);
        alSourcePlay(it->second.source);
    }

    // Configurar fade
    fadeStates[name] = { FadeState::FADING_IN, 0.0f, duration, loop };
}

void AudioManager::fadeOut(const std::string& name, float duration)
{
    auto it = clips.find(name);
    if (it == clips.end())
    {
        printf("[Audio] Error: '%s' no esta cargado.\n", name.c_str());
        return;
    }

    if (!isPlaying(name))
        return;

    // Configurar fade
    fadeStates[name] = { FadeState::FADING_OUT, 0.0f, duration, false };
}

void AudioManager::update(float deltaTime)
{
    for (auto it = fadeStates.begin(); it != fadeStates.end(); )
    {
        const std::string& name = it->first;
        FadeInfo& fadeInfo = it->second;

        if (fadeInfo.state == FadeState::NONE)
        {
            ++it;
            continue;
        }

        fadeInfo.currentTime += deltaTime;
        float progress = glm::clamp(fadeInfo.currentTime / fadeInfo.duration, 0.0f, 1.0f);

        auto clipIt = clips.find(name);
        if (clipIt != clips.end())
        {
            if (fadeInfo.state == FadeState::FADING_IN)
            {
                alSourcef(clipIt->second.source, AL_GAIN, progress);

                if (progress >= 1.0f)
                {
                    fadeInfo.state = FadeState::NONE;
                }
            }
            else if (fadeInfo.state == FadeState::FADING_OUT)
            {
                float currentGain = 1.0f - progress;
                alSourcef(clipIt->second.source, AL_GAIN, currentGain);

                if (progress >= 1.0f)
                {
                    alSourcePause(clipIt->second.source);
                    fadeInfo.state = FadeState::NONE;
                }
            }
        }

        ++it;
    }
}

// Audio 3D
void AudioManager::setSourcePosition(const std::string& name, const glm::vec3& pos)
{
    auto it = clips.find(name);
    if (it != clips.end())
        alSource3f(it->second.source, AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioManager::setListenerPosition(const glm::vec3& position,
    const glm::vec3& front,
    const glm::vec3& up)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);

    ALfloat orientation[] = {
        front.x, front.y, front.z,
        up.x,    up.y,    up.z
    };
    alListenerfv(AL_ORIENTATION, orientation);
}

// Métodos auxiliares
ALuint AudioManager::createSource(ALuint buffer, bool loop, float gain)
{
    ALuint source;
    alGenSources(1, &source);

    // Asociar el buffer de audio a esta source
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcef(source, AL_GAIN, gain);
    alSourcef(source, AL_PITCH, 1.0f);

    // Posicion inicial en el origen, se actualiza con setSourcePosition()
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    checkError("createSource");
    return source;
}

void AudioManager::checkError(const std::string& where) const
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
        printf("[Audio] Error en '%s': 0x%X\n", where.c_str(), error);
}

// Limpieza
void AudioManager::shutdown()
{
    // Liberar todas las sources y buffers en GPU de audio
    for (auto& [name, clip] : clips)
    {
        if (clip.source)
        {
            alSourceStop(clip.source);
            alDeleteSources(1, &clip.source);
        }
        if (clip.buffer)
            alDeleteBuffers(1, &clip.buffer);
    }
    clips.clear();

    // Destruir contexto y cerrar dispositivo
    if (context)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }
    if (device)
    {
        alcCloseDevice(device);
        device = nullptr;
    }
}

AudioManager::~AudioManager()
{
    shutdown();
}