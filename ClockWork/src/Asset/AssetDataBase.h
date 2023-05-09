#pragma once
#include <unordered_map>

namespace CW
{
	class AssetDataBase
	{





	private:
		std::unordered_map<const char*, Asset*> _assets;
		const char* _assetFolder;

	};

	class Model : public Asset
	{
	
	};

	class Animation
	{

	};


	class Texture
	{

	};

	class Shader
	{

	};


	class Asset
	{
		virtual void Load(const char* path) = 0;
	};

}