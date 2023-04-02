#include "RenderSystem.h"


namespace CW
{
	void CW::RenderSystem::Update(float deltaTime)
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto& ecs = ECS::Instance();
		for (auto& entity : _entities)
		{
			auto& camera = ecs.GetSingleton_Camera();
			auto& renderable = ecs.GetComponent<RenderableComponent>(entity);
			auto& transform = ecs.GetComponent<TransformComponent>(entity);

			for (auto& meshId : renderable.MeshIds)
			{
				auto& mesh = ecs.GetAsset<MeshComponent>(meshId);
				Render(mesh, transform, camera);
			}

			/*for (auto& mesh : renderable.Meshes)
			{
				Render(mesh, transform, camera);
			}*/
		}
	}

	void RenderSystem::Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera)
	{
		mesh.Shader.Use();

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		auto camMat = camera.CameraMatrix();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Position);
		glm::quat rotationQuat = glm::quat(transform.Rotation);
		glm::mat quatMatrix = glm::toMat4(rotationQuat);
		model *= quatMatrix;
		model = glm::scale(model,transform.Scale);

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

}