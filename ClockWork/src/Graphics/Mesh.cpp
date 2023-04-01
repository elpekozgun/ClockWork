#include "Mesh.h"


namespace CW
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
		: Vertices(vertices), Indices(indices), Textures(textures)
	{
		Setup();
	}

	Mesh::~Mesh()
	{

	}


	void Mesh::Delete()
	{
		glDeleteVertexArrays(1,&_vao.Id);
		for (auto& texture : Textures)
		{
			texture.Delete();
		}
	}

	void Mesh::Setup()
	{
		_vao.Bind();

		EBO ebo(Indices);
		VBO vbo(Vertices);

		unsigned int stride = sizeof(Vertex) / sizeof(float);

		_vao.LinkAttribArray<float>(vbo, 0, 3, GL_FLOAT, stride, 0);
		_vao.LinkAttribArray<float>(vbo, 1, 3, GL_FLOAT, stride, 3);
		_vao.LinkAttribArray<float>(vbo, 2, 3, GL_FLOAT, stride, 6);
		_vao.LinkAttribArray<float>(vbo, 3, 2, GL_FLOAT, stride, 9);

		_vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();
	}

	void Mesh::Draw(Shader& shader, glm::mat4 camMat, glm::vec3 camPos)
	{
		shader.Use();
		unsigned int diffuseNo = 0;
		unsigned int specularNo = 0;
		for (unsigned int i = 0; i < Textures.size(); i++)
		{
			int no = 0;
			std::string name = Textures[i].TextureType;
			if (name == "Diffuse")
				no = diffuseNo++;
			else if (name == "Specular")
				no = specularNo++;

			std::string fullName = name + std::to_string(no);

			shader.SetTexture(fullName, i);

			Textures[i].Bind();
		}

		_vao.Bind();
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	}
}
