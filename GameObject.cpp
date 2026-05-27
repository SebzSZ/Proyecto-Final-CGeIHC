#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "PointLight.h"
#include "Shader.h"
#include "SpotLight.h"

GameObject::GameObject(const std::string& name)
	: name(name)
{}

// Jerarquía
void GameObject::addChild(std::shared_ptr<GameObject> child)
{
	children.push_back(child);
}

// Geometría
void GameObject::setMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void GameObject::setModel(Model* model)
{
    this->model = model;
}

// Componentes
void GameObject::setMaterial(Material* material)
{
    this->material = material;
}

void GameObject::setPointLight(PointLight* pointLight)
{
    this->pointLight = pointLight;
}

void GameObject::setSpotLight(SpotLight* spotLight)
{
    this->spotLight = spotLight;
}

void GameObject::setTextureID(GLuint id)
{
	textureID = id;
	hasTexture = (id != 0);
}

// Renderizado
void GameObject::draw(Shader& shader, const glm::mat4& parentMatrix)
{
    if (!visible) return;

	glm::mat4 worldMatrix = transform.getModelMatrix(parentMatrix);
    
    drawSelf(shader, worldMatrix);

    for (const auto& child : children)
        child->draw(shader, worldMatrix);
}

void GameObject::drawSelf(Shader& shader, const glm::mat4& worldMatrix)
{
	syncLightPosition(worldMatrix);

	if (!model && !mesh) return;

	glUniformMatrix4fv(shader.getModelLocation(), 1, GL_FALSE, glm::value_ptr(worldMatrix));

    if (material)
		material->use(shader.getSpecularIntensityLocation(), shader.getShininessLocation());

    if (hasTexture)
    {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
    }

	if (model)
		model->render();
	else
		mesh->render();

    if (hasTexture)
		glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::syncLightPosition(const glm::mat4& worldMatrix)
{
	if (!pointLight && !spotLight) return;

	glm::vec3 worldPosition = glm::vec3(worldMatrix[3]);

    if (pointLight)
		pointLight->setPosition(worldPosition); 

	if (spotLight)
	{
		spotLight->setPosition(worldPosition);
		glm::vec3 forward = glm::normalize(glm::vec3(worldMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
		spotLight->setDirection(forward);
	}
}

GameObject::~GameObject()
{}