#include "Model.h"

namespace CW
{
	Model::Model(const char* file) : _file(file)
	{
		LoadModel(file);
	}

	void Model::Draw(Shader& shader, glm::mat4 camMat, glm::vec3 camPos) 
	{
		for (auto& mesh : Meshes)
		{
			mesh.Draw(shader, camMat, camPos);
		}
	}

	void Model::LoadModel(const std::string& path)
	{
		// aiProcess_SplitLargeMeshes-> bigger meshes to smaller ones for max mesh element count optimization
		// aiPriocess_GenNormals -> generates normals from vertex data, ez.
		// aiProcess_OptimizeMeshes -> Smaller meshes into bigger ones to reduce draw calls.

		Assimp::Importer importer;				  // convert faces to triangles   // flip UVS for opengl
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "import error: " << importer.GetErrorString() << "\n";
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		
		//unsigned int pos = directory.find('\\');


		ProcessNode(scene->mRootNode, scene);
	}
	
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 data;
			data.x = mesh->mVertices[i].x;
			data.y = mesh->mVertices[i].y;
			data.z = mesh->mVertices[i].z;
			vertex.Position = data;

			data.x = mesh->mNormals[i].x;
			data.y = mesh->mNormals[i].y;
			data.z = mesh->mNormals[i].z;
			vertex.Normal = data;

			vertex.Color = glm::vec3(0);

			if (mesh->mTextureCoords[0])
			{
				glm::vec3 data;
				data.x = mesh->mTextureCoords[0][i].x;
				data.y = mesh->mTextureCoords[0][i].y;
				vertex.UV = data;
			}
			else
			{
				vertex.UV = glm::vec2(0);
			}

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
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "Diffuse", slot);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "Specular", slot);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}
	
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name, unsigned int& slot)
	{
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			auto path = directory + "/" + str.C_Str();

			Texture texture(path.c_str(), name, i+(slot++));

			textures.push_back(texture);
		}

		return textures;
	}

}