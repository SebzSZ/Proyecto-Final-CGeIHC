#include <cstring>
#include <cmath>
#include "Mesh.h"

Mesh::Mesh()
	: VAO(0), VBO(0), EBO(0), indexCount(0)
{}

void Mesh::create(GLfloat* vertices, GLuint* indices, GLuint vertexCount, GLuint indexCount)
{
	this->indexCount = static_cast<GLsizei>(indexCount);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	constexpr GLsizei stride = 8 * sizeof(GLfloat);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::calcAverageNormals(GLuint* indices, GLuint indexCount, GLfloat* vertices, GLuint vertexCount, GLuint vLength, GLuint normalOffset)
{
	for (GLuint i = 0; i < indexCount; i += 3)
	{
		GLuint i0 = indices[i] * vLength;
		GLuint i1 = indices[i + 1] * vLength;
		GLuint i2 = indices[i + 2] * vLength;

		// Vectores de dos aristas de la cara
		GLfloat v1x = vertices[i1] - vertices[i0];
		GLfloat v1y = vertices[i1 + 1] - vertices[i0 + 1];
		GLfloat v1z = vertices[i1 + 2] - vertices[i0 + 2];

		GLfloat v2x = vertices[i2] - vertices[i0];
		GLfloat v2y = vertices[i2 + 1] - vertices[i0 + 1];
		GLfloat v2z = vertices[i2 + 2] - vertices[i0 + 2];

		// Producto cruz para la normal
		GLfloat nx = v1y * v2z - v1z * v2y;
		GLfloat ny = v1z * v2x - v1x * v2z;
		GLfloat nz = v1x * v2y - v1y * v2x;

		// Acumular en cada vértice de la cara
		vertices[i0 + normalOffset] += nx;
		vertices[i0 + normalOffset + 1] += ny;
		vertices[i0 + normalOffset + 2] += nz;

		vertices[i1 + normalOffset] += nx;
		vertices[i1 + normalOffset + 1] += ny;
		vertices[i1 + normalOffset + 2] += nz;

		vertices[i2 + normalOffset] += nx;
		vertices[i2 + normalOffset + 1] += ny;
		vertices[i2 + normalOffset + 2] += nz;
	}

	// Normalizar las normales acumuladas
	for (GLuint i = 0; i < vertexCount / vLength; i++)
	{
		GLuint base = i * vLength + normalOffset;
		GLfloat nx = vertices[base];
		GLfloat ny = vertices[base + 1];
		GLfloat nz = vertices[base + 2];

		GLfloat length = sqrtf(nx * nx + ny * ny + nz * nz);
		if (length > 0.0f)
		{
			vertices[base] = nx / length;
			vertices[base + 1] = ny / length;
			vertices[base + 2] = nz / length;
		}
	}
}

void Mesh::render() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::clear()
{
	if (EBO) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh()
{
	clear();
}