#pragma once

#include <string>
#include <unordered_map>

#include <al.h>
#include <alc.h>
#include <glm.hpp>

class AudioManager
{
public:
	// Singleton
	static AudioManager& getInstance()
	{
		static AudioManager instance;
		return instance;
	}

	// Inicialización
	bool Initialize();

	// Carga de sonidos 
	bool loadWAV(const std::string& name, const std::string& filePath);
	bool loadMP3(const std::string& name, const std::string& filePath);

	// Reproducción y control
	void play(const std::string& name, bool loop = false, float gain = 1.0f);
	void stop(const std::string& name);
	void pause(const std::string& name);
	void resume(const std::string& name);
	void setGain(const std::string& name, float gain);
	bool isPlaying(const std::string& name) const;

	// Audio 3D
	void setSourcePosition(const std::string& name, const glm::vec3& pos);
	void setListenerPosition(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

	// Limpieza
	void shutdown();
	~AudioManager();

private:
	// Constructores ocultos para el singleton
	AudioManager() = default;
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	// Estructuras internas
	struct AudioClip
	{
		ALuint buffer = 0;
		ALuint source = 0;
	};

	// Variables de OpenAL
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

	// Almacenamiento de sonidos
	std::unordered_map<std::string, AudioClip> clips;

	// Métodos auxiliares
	ALuint createSource(ALuint buffer, bool loop, float gain);
	void checkError(const std::string& where) const;
};