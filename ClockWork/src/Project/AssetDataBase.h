#pragma once

#include <unordered_map>
#include <iostream>
#include <fstream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Graphics/AssimpHelper.h"
#include "stb_image.h"
#include "Graphics/Vertex.h"
#include "Json.h"
#include "ModelLoader.h"


using json = nlohmann::json;

namespace CW
{
	class CW_API Asset
	{
		virtual void Serialize(const char* path) = 0;
	};


	class MeshData
	{
	public:
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<std::string> Textures;
	};

	struct BoneData
	{
		int id;
		glm::mat4 offset;
	};


	class ModelData 
	{
	public:
		std::list<MeshData> Meshes;
		int BoneCount = 0;
		std::unordered_map<std::string, BoneData> BoneMap;
	};

	class AnimationData
	{
		public:
	};

	class SoundData
	{

	};


	
	class CW_API AssetDataBase
	{

	public:
		static ModelData LoadModel(const std::string& path);
		static AnimationData LoadAnimation(const std::string& path);
		static ModelData ImportModel(const std::string& path);

	private:
		static void ProcessNode(ModelData& model, aiNode* node, const aiScene* scene);
		static MeshData CreateMesh(ModelData& model, aiMesh* mesh, const aiScene* scene);
		static std::vector<std::string> GetTexturePaths(aiMaterial* mat, aiTextureType type);
		static void SetVertexBoneData(Vertex& vertex, int boneId, float weight);
		static void SetVertexDefaultBoneData(Vertex& vertex);
		static void ExtractVertexBoneWeights(ModelData& model, std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

;		std::unordered_map<const char*, Asset*> _assets;
		const char* _assetFolder;


		static Assimp::Importer importer;
		//static ModelLoader modelLoader;
	};

}