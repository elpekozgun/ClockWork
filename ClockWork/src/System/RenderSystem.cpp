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

			// TODO: enqueue the items that can be instanced into a container,
			// while traversing below loop add those transformations to the container,
			// after the traversal is over render them instanced all at once.

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

		auto camMat = camera.CameraMatrix();

		glm::mat4 model = MatrixFromTransform(transform);

		mesh.Shader.SetBool("instanced", true);

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

	// havent increased performance, on the contrary dropped huge.
	void RenderSystem::RenderInstanced(std::map<unsigned int, std::vector<glm::mat4>>& transformMap, CameraComponent& camera)
	{
		auto& ecs = ECS::Instance();

		for (auto& pair : transformMap)
		{
			auto meshId = pair.first;
			auto transforms = pair.second;

			auto& mesh = ecs.GetAsset<MeshComponent>(meshId);

			mesh.Vao.Bind();
			mesh.Shader.Use();
			mesh.Shader.SetBool("instanced", false);

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
}