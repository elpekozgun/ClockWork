#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Graphics/AssimpHelper.h"

namespace CW
{
	class ModelLoader
	{
	public:
		//static void ProcessNode(aiNode* node, const aiScene* scene);
		//static MeshData CreateMesh(aiMesh* mesh, const aiScene* scene);


		Assimp::Importer importer;
	};
}