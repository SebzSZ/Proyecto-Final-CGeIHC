#include <cstdio>
#include "Model.h"

Model::Model()
{}

bool Model::load(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		printf("[Model] Error: No se pudo cargar '%s': %s\n", filePath.c_str(), importer.GetErrorString());
		return false;
	}

	std::string modelDir = filePath.substr(0, filePath.find_last_of("/\\"));

	processNode(scene->mRootNode, scene, modelDir);
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene, const std::string& modelDir)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aiMeshPtr = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(aiMeshPtr, scene, modelDir));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, modelDir);
	}
}

Mesh* Model::processMesh(aiMesh* aiMeshPtr, const aiScene* scene, const std::string& modelDir)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i < aiMeshPtr->mNumVertices; i++)
	{
		// Posición
		vertices.push_back(aiMeshPtr->mVertices[i].x);
		vertices.push_back(aiMeshPtr->mVertices[i].y);
		vertices.push_back(aiMeshPtr->mVertices[i].z);

		// UV
		if (aiMeshPtr->mTextureCoords[0])
		{
			vertices.push_back(aiMeshPtr->mTextureCoords[0][i].x);
			vertices.push_back(aiMeshPtr->mTextureCoords[0][i].y);
		}
		else
		{
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		// Normales
		if (aiMeshPtr->mNormals)
		{
			vertices.push_back(aiMeshPtr->mNormals[i].x);
			vertices.push_back(aiMeshPtr->mNormals[i].y);
			vertices.push_back(aiMeshPtr->mNormals[i].z);
		}
		else
		{
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
		}
	}

	for (unsigned int i = 0; i < aiMeshPtr->mNumFaces; i++)
	{
		aiFace& face = aiMeshPtr->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* mesh = new Mesh();
	mesh->create(vertices.data(), indices.data(), (GLuint)vertices.size(), (GLuint)indices.size());

	if (aiMeshPtr->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[aiMeshPtr->mMaterialIndex];
		Texture* texture = loadMaterialTexture(material, modelDir);
		textures.push_back(texture);
	}
	else
	{
		textures.push_back(nullptr);
	}

	return mesh;
}

Texture* Model::loadMaterialTexture(aiMaterial* material, const std::string& modelDir)
{
	if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0) return nullptr;

	aiString path;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	std::string fullPath = std::string(path.C_Str());
	size_t slashPos = fullPath.find_last_of("/\\");
	std::string filename = (slashPos != std::string::npos)
		? fullPath.substr(slashPos + 1)
		: fullPath;

	std::string texPath = "Textures/" + filename;

	Texture* texture = new Texture(texPath.c_str());

	std::string ext = filename.substr(filename.find_last_of('.') + 1);
	bool loaded = (ext == "tga" || ext == "png")
		? texture->loadWithAlpha()
		: texture->load();

	if (!loaded) { delete texture; return nullptr; }
	return texture;
}

void Model::render() const
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (i < textures.size() && textures[i])
		{
			textures[i]->bind();
		}
		meshes[i]->render();

		if (i < textures.size() && textures[i])
		{
			textures[i]->unbind();
		}
	}
}

void Model::clear()
{
	for (Mesh* mesh : meshes)
	{
		mesh->clear();
		delete mesh;
	}

	for (Texture* texture : textures)
	{
		if (texture)
		{
			texture->clear();
			delete texture;
		}
	}

	meshes.clear();
	textures.clear();
}

Model::~Model()
{
	clear();
}