#pragma once
#include <string>
#include <unordered_map>

#include <al.h>
#include <alc.h>
#include <glm.hpp>


class AudioManager
{
public:
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }


    bool Initialize();


    void shutdown();

    // Carga de sonidos 

    bool loadWAV(const std::string& name, const std::string& filePath);

    // Carga un archivo .mp3 y lo almacena con el nombre dado.
    bool loadMP3(const std::string& name, const std::string& filePath);

    // Reproduccion 
    void play(const std::string& name, bool loop = false, float gain = 1.0f);

    // Detiene la reproduccion de un sonido.
    void stop(const std::string& name);

    // Pausa y reanuda un sonido.
    void pause(const std::string& name);
    void resume(const std::string& name);

    // Cambia el volumen de un sonido en reproduccion.
    void setGain(const std::string& name, float gain);

    // Devuelve true si el sonido esta reproduciendose actualmente.
    bool isPlaying(const std::string& name) const;

    //Audio 3D

    void setSourcePosition(const std::string& name, const glm::vec3& pos);

    void setListenerPosition(const glm::vec3& position,
        const glm::vec3& front,
        const glm::vec3& up);

    ~AudioManager();

private:
    AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;


    struct AudioClip
    {
        ALuint buffer = 0;
        ALuint source = 0;
    };

    std::unordered_map<std::string, AudioClip> clips;


    ALuint createSource(ALuint buffer, bool loop, float gain);


    void checkError(const std::string& where) const;
};
