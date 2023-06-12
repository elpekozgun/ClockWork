#include "Model.h"

namespace CW
{
	Model::Model(const char* file) : _file(file)
	{
		LoadModel(file);
	}

	void Model::LoadModel(const std::string& path)
	{
		// aiProcess_SplitLargeMeshes-> bigger meshes to smaller ones for max mesh element count optimization
		// aiPriocess_GenNormals -> generates normals from vertex data, ez.
		// aiProcess_OptimizeMeshes -> Smaller meshes into bigger ones to reduce draw calls.
							     				  // convert faces to triangles   // flip UVS for opengl
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "import error: " << importer.GetErrorString() << "\n";
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		
		
		ProcessNode(scene->mRootNode, scene);

	}
	
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//Meshes.push_back(ProcessMesh(mesh, scene));
			ModelData.Meshes.push_back(CreateMesh(mesh, scene));
			MeshComponents.push_back(CreateMeshComponent(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}	

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name, unsigned int& slot)
	{
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			aiReturn a = mat->GetTexture(type, i, &str);


			auto path = directory + "/" + str.C_Str();

			Texture texture(path.c_str(), name, i + (slot++));

			textures.push_back(texture);
		}

		return textures;
	}

	std::vector<std::string> Model::GetTexturePaths(aiMaterial* mat, aiTextureType type)
	{
		std::vector<std::string> paths;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			aiReturn a = mat->GetTexture(type, i, &str);
			auto path = directory + "/" + str.C_Str();
			paths.push_back(path);
		}

		return paths;
	}

	MeshData Model::CreateMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::string> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			SetVertexBoneDataToDefault(vertex);

			glm::vec3 data;
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

		ExtractBoneWeightForVertices(vertices, mesh, scene);

		MeshData data;
		data.Indices = indices;
		data.Vertices = vertices;
		data.Textures = textures;

		return data;
	}

	MeshComponent Model::CreateMeshComponent(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			SetVertexBoneDataToDefault(vertex);

			glm::vec3 data;
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
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "Diffuse", slot);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "Specular", slot);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "Normal", slot);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<Texture> metalMaps = LoadMaterialTextures(material, aiTextureType_METALNESS, "Metallic", slot);
			textures.insert(textures.end(), metalMaps.begin(), metalMaps.end());

			std::vector<Texture> roughnessMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "Roughness", slot);
			textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
		}

		ExtractBoneWeightForVertices(vertices, mesh, scene);

		std::vector<int>  boneStuff;

		std::vector<float> weights;

		for (size_t i = 0; i < vertices.size(); i++)
		{
			float weight = 0.0f;
			weight += vertices[i].Weights[0];
			weight += vertices[i].Weights[1];
			weight += vertices[i].Weights[2];
			weight += vertices[i].Weights[3];
			if (weight > 1.0)
			{
				weights.push_back(weight);
			}
			

			if (vertices[i].BoneIds[0] > 99)
				boneStuff.push_back(vertices[i].BoneIds[0]);
			if (vertices[i].BoneIds[1] > 99)
				boneStuff.push_back(vertices[i].BoneIds[1]);
			if (vertices[i].BoneIds[2] > 99)
				boneStuff.push_back(vertices[i].BoneIds[2]);
			if (vertices[i].BoneIds[3] > 99)
				boneStuff.push_back(vertices[i].BoneIds[3]);
		}

		//AssetDataBase::

		VAO vao;
		vao.Bind();

		EBO ebo(indices);
		VBO vbo(vertices);

		unsigned int stride = sizeof(Vertex) / sizeof(float);

		vao.LinkAttribArray<float>(vbo, 0, 3, GL_FLOAT, stride, 0);
		vao.LinkAttribArray<float>(vbo, 1, 3, GL_FLOAT, stride, 3);
		vao.LinkAttribArray<float>(vbo, 2, 2, GL_FLOAT, stride, 6);
		vao.LinkAttribArray<float>(vbo, 3, 3, GL_FLOAT, stride, 8);
		vao.LinkAttribArray<float>(vbo, 4, 4, GL_FLOAT, stride, 11);
		vao.LinkAttribArray<float>(vbo, 5, 4, GL_FLOAT, stride, 15);

		//vbo.Bind();

		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, BoneIds)));
		//
		//glEnableVertexAttribArray(6);
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,Weights)));


		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();

		float minX = std::numeric_limits<float>::max();
		float minY = minX;
		float minZ = minX;

		float maxX = std::numeric_limits<float>::lowest();
		float maxY = maxX;
		float maxZ = maxX;

		for (const Vertex& vertex : vertices)
		{
			maxX = std::max(maxX, vertex.Position.x);
			minX = std::min(minX, vertex.Position.x);
			maxY = std::max(maxY, vertex.Position.y);
			minY = std::min(minY, vertex.Position.y);
			maxZ = std::max(maxZ, vertex.Position.z);
			minZ = std::min(minZ, vertex.Position.z);
		}

		MeshComponent meshComponent;
		meshComponent.Vertices = vertices;
		meshComponent.Indices = indices;
		meshComponent.Textures = textures;
		meshComponent.VaoId = vao.Id;
		meshComponent.AABB = AABB{ maxX,minX,maxY,minY,maxZ,minZ };
		meshComponent.Color = vec3(0.35f);

		return meshComponent;
	}
}