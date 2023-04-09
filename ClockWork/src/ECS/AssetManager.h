#pragma once 
#include "ComponentArray.h"


#include "ECS/ECS.h"
#include "Component.h"

#include <unordered_map>
#include <set>

namespace CW
{

	class CW_API IAssetArray
	{
	public:
		virtual ~IAssetArray() = default;
		virtual void DestroyAsset(uint assetId) = 0;
	};

	template<typename T>
	class AssetArray : public IAssetArray
	{
	public:
		uint AddData(T& asset)
		{
			_assetData.emplace_back(asset);
			_size++; 

			return _size - 1;
		}

		void RemoveData(uint assetId)
		{
			// later
		}

		T& GetData(uint assetId)
		{
			return _assetData[assetId];
		}

		void DestroyAsset(uint assetId) override
		{
			RemoveData(assetId);
		}


	private:

		std::vector<T> _assetData;
		//T _assetData[MAX_ENTITY];

		//std::unordered_map<unsigned int, T> _assetData;
		/*T _assetArray[MAX_ENTITY];
		std::unordered_map<uint, uint> _assetToIndex{};
		std::unordered_map<uint, uint> _indexToAsset{};*/

		uint _size = 0;
	};
	

	class CW_API AssetManager
	{
	public:

		template<typename T>
		uint AddAsset(T& asset)
		{
			std::string typeName = typeid(T).name();

			if (_assetArrays.find(typeName) == _assetArrays.end())
			{
				_assetArrays.insert({ typeName, std::make_shared<AssetArray<T>>() });
			}

			return GetAssetArray<T>()->AddData(asset);
		}

		template<typename T>
		T& GetAsset(uint id)
		{
			return GetAssetArray<T>()->GetData(id);
		}

		template<typename T = MeshComponent>
		void MakeInstanced(unsigned int meshId, std::vector<glm::mat4>& transforms)
		{
			MeshComponent& mesh = GetAsset<MeshComponent>(meshId);

			mesh.Vao.Bind();

			VBO instanceVBO(transforms);

			instanceVBO.Bind();
			mesh.Vao.AddInstanceBuffers(instanceVBO);
			mesh.Vao.Unbind();
			instanceVBO.Unbind();

			mesh.instanceVbo = instanceVBO.Id;
		}


	private:

		template<typename T>
		std::shared_ptr<AssetArray<T>> GetAssetArray()
		{
			std::string typeName = typeid(T).name();
			return std::static_pointer_cast<AssetArray<T>>(_assetArrays[typeName]);
		}

		std::unordered_map<std::string, std::shared_ptr<IAssetArray>> _assetArrays;
	};
}