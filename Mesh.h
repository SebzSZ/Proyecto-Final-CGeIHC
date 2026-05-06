#pragma once

#include <glew.h>

class Mesh
{
public:
	Mesh();

	void create(GLfloat* vertices, GLuint* indices, GLuint vertexCount, GLuint indexCount);

	static void calcAverageNormals(GLuint* indices, GLuint indexCount, GLfloat* vertices, GLuint vertexCount, GLuint vLength, GLuint normalOffset);

	void render() const;

	void clear();
	~Mesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};