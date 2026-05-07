#include "Material.h"

Material::Material()
	: specularIntensity(0.0f), shininess(0.0f)
{
}

Material::Material(GLfloat specularIntensity, GLfloat shininess)
	: specularIntensity(specularIntensity), shininess(shininess)
{
}

void Material::use(GLuint specularIntensityLocation, GLuint shininessLocation) const
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}