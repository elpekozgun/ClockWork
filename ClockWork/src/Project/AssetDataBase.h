#pragma once

#include <unordered_map>
#include "stb_image.h"
#include "Graphics/Vertex.h"
#include "Json.h"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

namespace CW
{
	class CW_API Asset
	{
		virtual void Load(const char* path) = 0;
	};


	class MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<std::string> Textures;
	};

	class ModelData 
	{
		std::list<MeshData> Meshes;
	};

	

	//class Animation
	//{
	//};


	//class Texture
	//{

	//};

	//class Shader
	//{

	//};

	class CW_API AssetDataBase
	{

	public:
		static ModelData LoadModel(const char* path);
		//{
		//	std::ifstream file(path);

		//	//std::string filecontents(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		//	json jsonData;

		//	jsonData = json::parse(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		//	auto name = jsonData["textures"];

		//}



	private:
		std::unordered_map<const char*, Asset*> _assets;
		const char* _assetFolder;

	};

}