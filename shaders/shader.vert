#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

// Salidas para el fragment shader
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 VertexColor;

// Uniforms
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 color;
uniform vec2 textureOffset;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	FragPos = vec3(model * vec4(pos, 1.0));

	Normal = mat3(transpose(inverse(model))) * norm;

	TexCoords = tex + textureOffset;

	VertexColor = vec4(color, 1.0);
}
