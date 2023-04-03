#include "RenderSystem.h"


namespace CW
{
	void CW::RenderSystem::Update(float deltaTime)
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;
		auto& ecs = ECS::Instance();
		
		auto& camera = ecs.GetSingleton_Camera();

		for (auto& entity : _entities)
		{
			auto& renderable = ecs.GetComponent<RenderableComponent>(entity);
			auto& transform = ecs.GetComponent<TransformComponent>(entity);

			for (auto& meshId : renderable.MeshIds)
			{
				if (renderable.Instanced)
				{
					instanceTranslations[meshId].push_back(MatrixFromTransform(transform));
				}
				else
				{
					auto& mesh = ecs.GetAsset<MeshComponent>(meshId);
					Render(mesh, transform, camera);
				}
			}
		}


		RenderInstanced(instanceTranslations, camera);
	}

	void RenderSystem::Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera)
	{
		mesh.Shader.Use();

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 model = MatrixFromTransform(transform);  //transform.GetMatrix();
		auto camMat = CameraMat(camera);

		mesh.Shader.SetBool("instanced", false);

		mesh.Shader.setVec3("spotlight.position", camera.Position);
		mesh.Shader.setVec3("spotlight.direction", camera.Forward);
		mesh.Shader.setMat4("Model", model);
		mesh.Shader.setMat4("CamMat", camMat);
		mesh.Shader.setVec3("eyePosition", camera.Position);

		unsigned int diffuseNo = 0;
		unsigned int specularNo = 0;
		for (unsigned int i = 0; i < mesh.Textures.size(); i++)
		{
			int no = 0;
			std::string name = mesh.Textures[i].TextureType;
			if (name == "Diffuse")
				no = diffuseNo++;
			else if (name == "Specular")
				no = specularNo++;

			std::string fullName = name + std::to_string(no);

			mesh.Shader.SetTexture(fullName, i);

			mesh.Textures[i].Bind();
		}

		mesh.Vao.Bind();
		glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);
	}

	// on mesh with 46k tris it doesnt increase the performance in release mode, 
	// however on 30k elements of quads it boosts 3x speed. (10fps to 30fps)
	// maybe we can do a paged instancing instead of pushing all at once for complex geometry??
	void RenderSystem::RenderInstanced(std::map<unsigned int, std::vector<glm::mat4>>& transformMap, CameraComponent& camera)
	{
		auto& ecs = ECS::Instance();

		for (auto& pair : transformMap)
		{
			auto meshId = pair.first;
			auto transforms = pair.second;

			auto& mesh = ecs.GetAsset<MeshComponent>(meshId);

			mesh.Shader.Use();


			mesh.Shader.SetBool("instanced", true);

			glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			auto camMat = camera.CameraMatrix();

			mesh.Shader.setMat4("CamMat", camMat);
			mesh.Shader.setVec3("eyePosition", camera.Position);
			mesh.Shader.setVec3("spotlight.position", camera.Position);
			mesh.Shader.setVec3("spotlight.direction", camera.Forward);

			unsigned int diffuseNo = 0;
			unsigned int specularNo = 0;
			for (unsigned int i = 0; i < mesh.Textures.size(); i++)
			{
				int no = 0;
				std::string name = mesh.Textures[i].TextureType;
				if (name == "Diffuse")
					no = diffuseNo++;
				else if (name == "Specular")
					no = specularNo++;

				std::string fullName = name + std::to_string(no);

				mesh.Shader.SetTexture(fullName, i);

				mesh.Textures[i].Bind();
			}

			mesh.Vao.Bind();
			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVbo);
			glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

			glDrawElementsInstanced(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
		}
	}

	glm::mat4 RenderSystem::MatrixFromTransform(TransformComponent& transform	)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Position);
		glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
		glm::mat quatMatrix = glm::toMat4(rotation);
		model *= quatMatrix;
		model = glm::scale(model, transform.Scale);
		return model;
	}

	glm::mat4 RenderSystem::CameraMat(CameraComponent& camera)
	{
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);

		view = glm::lookAt(camera.Position, camera.Position + camera.Forward, camera.Up);
		projection = glm::perspective(glm::radians(camera.FoV), (float)camera.Width / camera.height, 0.1f, 100.0f);

		auto right = glm::normalize(glm::cross(camera.Forward, camera.Up));

		return projection * view;
	}

}