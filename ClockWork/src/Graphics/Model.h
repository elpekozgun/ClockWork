#pragma once

#include "Core/Core.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ECS/Component.h"

#include <iostream>
#include <vector>
#include <string>

namespace CW
{
	class CW_API Model
	{
	public:
		Model(const char* file);
		
		std::vector<MeshComponent> MeshComponents;


	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		MeshComponent CreateMeshComponent(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name, unsigned int& slot);

		std::string directory;

		unsigned int _instanceCount = 0;
		const char* _file;
		std::vector<unsigned char> data;

		Assimp::Importer importer;
	};
}