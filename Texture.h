#pragma once

#include <string>
#include <glew.h>

class Texture
{
public:
	Texture();
	explicit Texture(const char* filePath);

	bool load();
	bool loadWithAlpha();

	void bind() const;
	void unbind() const;

	GLuint getID() const { return textureID; }

	void clear();
	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	std::string filePath;
};