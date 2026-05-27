#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Transform.h"

class Material;
class Mesh;
class Model;
class PointLight;
class Shader;
class SpotLight;

class GameObject
{
public:
	explicit GameObject(const std::string& name);

	const std::string& getName() const { return name; }

	Transform transform;

	// Jerarquía
	void addChild(std::shared_ptr<GameObject> child);
	const std::vector<std::shared_ptr<GameObject>>& getChildren() const { return children; }

	// Geometría: No pertenecen a GameObject, su ciclo de vda depende quien los asigna
	void setMesh(Mesh* mesh);
	void setModel(Model* model);

	// Componentes: No pertenecen a GameObject, su ciclo de vda depende quien los asigna
	void setMaterial(Material* material);
	void setPointLight(PointLight* pointLight);
	void setSpotLight(SpotLight* spotLight);
	void setTextureID(GLuint id);

	bool isVisible() const { return visible; }
	void setVisible(bool visible) { this->visible = visible; }

	void draw(Shader& shader, const glm::mat4& parentMatrix = glm::mat4(1.0f));

	~GameObject();

private:
	std::string name;
	bool visible = true;

	std::vector<std::shared_ptr<GameObject>> children;

	// Punteros: Gestionados externamente
	Material* material = nullptr;
	Mesh* mesh = nullptr;
	Model* model = nullptr;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;

	GLuint textureID = 0;
	bool hasTexture = false;

	void drawSelf(Shader& shader, const glm::mat4& worldMatrix);
	void syncLightPosition(const glm::mat4& worldMatrix);
};