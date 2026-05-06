#version 330 core

// Entrada desde el vertex shader
in vec3 TexCoords;

// Salida del fragment shader
out vec4 FragColor;

// Uniform
uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}
