#include "AssetDataBase.h"

namespace CW
{
	


	ModelData AssetDataBase::LoadModel(const std::string& path)
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

	ModelData AssetDataBase::ImportModel(const std::string& path)
	{
		ModelData data;

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "import error: " << importer.GetErrorString() << "\n";
			return data;
		}

		ProcessNode(data, scene->mRootNode, scene);

		return data;
	}

	void AssetDataBase::ProcessNode(ModelData& model, aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model.Meshes.push_back(CreateMesh(model,mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(model,node->mChildren[i], scene);
		}
	}

	MeshData AssetDataBase::CreateMesh(ModelData& model, aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::string> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			SetVertexDefaultBoneData(vertex);
						
			vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
			vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
			vertex.Tangent = AssimpGLMHelpers::GetGLMVec(mesh->mTangents[i]);

			if (mesh->mTextureCoords[0])
				vertex.UV = AssimpGLMHelpers::GetGLMVec2(mesh->mTextureCoords[0][i]);
			else
				vertex.UV = glm::vec2(0);

			vertices.push_back(vertex);
		}

		// indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			unsigned int slot = 0;
			std::vector<std::string> diffuseMaps = GetTexturePaths(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<std::string> specularMaps = GetTexturePaths(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<std::string> normalMaps = GetTexturePaths(material, aiTextureType_NORMALS);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<std::string> metalMaps = GetTexturePaths(material, aiTextureType_METALNESS);
			textures.insert(textures.end(), metalMaps.begin(), metalMaps.end());

			std::vector<std::string> roughnessMaps = GetTexturePaths(material, aiTextureType_DIFFUSE_ROUGHNESS);
			textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
		}
		ExtractVertexBoneWeights(model, vertices, mesh, scene);

		MeshData data;
		data.Indices = indices;
		data.Vertices = vertices;
		data.Textures = textures;


		return data;
	}

	void AssetDataBase::SetVertexDefaultBoneData(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.BoneIds[i] = -1;
			vertex.Weights[i] = 0.0f;
		}
	}

	void AssetDataBase::SetVertexBoneData(Vertex& vertex, int boneId, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.BoneIds[i] < 0)
			{
				vertex.Weights[i] = weight;
				vertex.BoneIds[i] = boneId;
				break;
			}
		}
	}

	void AssetDataBase::ExtractVertexBoneWeights(ModelData& model, std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (model.BoneMap.find(boneName) == model.BoneMap.end())
			{
				BoneData boneData;
				boneData.id = model.BoneCount;
				boneData.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				model.BoneMap[boneName] = boneData;
				boneID = model.BoneCount;
				model.BoneCount++;
			}
			else
			{
				boneID = model.BoneMap[boneName].id;
			}
			assert(boneID != -1);
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= vertices.size());
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
	}

	std::vector<std::string> AssetDataBase::GetTexturePaths(aiMaterial* mat, aiTextureType type)
	{
		std::vector<std::string> paths;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			aiReturn a = mat->GetTexture(type, i, &str);
			paths.push_back(str.C_Str());
			//auto path = directory + "/" + str.C_Str();
			//paths.push_back(path);
		}

		return paths;
	}

	AnimationData AssetDataBase::LoadAnimation(const std::string& path)
	{
		return AnimationData();
	}


	Assimp::Importer AssetDataBase::importer;
}


