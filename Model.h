#pragma once

#include <string>
#include <vector>

#include <glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	bool load(const std::string& filePath);

	void render() const;

	void clear();
	~Model();

private:
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;

	void processNode(aiNode* node, const aiScene* scene, const std::string& modelDir);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene, const std::string& modelDir);
	Texture* loadMaterialTexture(aiMaterial* material, const std::string& modelDir);
};