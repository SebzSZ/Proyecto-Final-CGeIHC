#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 pos;

// Salidas para el fragment shader
out vec3 TexCoords;

// Uniforms
uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = pos;

	vec4 clipPos = projection * view * vec4(pos, 1.0);
	gl_Position = clipPos.xyww;
}
