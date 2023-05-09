#include "AssetDataBase.h"

namespace CW
{
	ModelData AssetDataBase::LoadModel(const char* path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cout << "file not found\n";
		}

		json jsonData;

		try
		{
			jsonData = json::parse(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

			json vertices = jsonData["vertices"];
			json indices = jsonData["indices"];
			json textures = jsonData["textures"];

			std::vector<glm::vec3> vertexData;
			for (const auto& vertex : vertices)
			{
				float x = vertex["x"];
				float y = vertex["y"];
				float z = vertex["z"];

				vertexData.push_back({ x, y, z });
			}

			std::vector<unsigned int> indexData;
			for (const auto& index : indices)
			{
				indexData.push_back(index);
			}

			std::vector<std::string> textureData;
			for (const auto& texture : textures)
			{
				textureData.push_back(texture);
			}
		}
		catch (const std::exception&)
		{
			std::cout << "cant read json dat\n";
		}


		return ModelData();
	}
}


