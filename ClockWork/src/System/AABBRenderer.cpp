#include "AABBRenderer.h"
#include <Core/Input.h>


namespace CW
{

	void AABBRenderer::Update(float dt)
	{
		bool showBox = _showBox;
		auto& input = Input::Instance();
		if (input.GetKeyPressed(CW::KEY_B))
		{
			_showBox = !_showBox;
		}

		if (showBox)
		{
			auto camera = _ecs->GetSingletonComponent<CameraComponent>();
			auto cameraMatrix = camera->CameraMatrix();

			auto transforms = _ecs->GetComponentArray<TransformComponent>();
			auto AAbbs = _ecs->GetComponentArray<AABBComponent>();

			for (auto& entity : _entities)
			{
				auto& aabb = AAbbs->GetData(entity);
				auto& transform = transforms->GetData(entity);
				Render(aabb, transform, *camera);
			}
		}
	}

	void AABBRenderer::Render(AABBComponent& aabb, TransformComponent& transform, CameraComponent& camera)
	{
		_aabbShader.Use();
		
		glm::mat4 model(1);
		glm::vec3 scale = aabb.Max - aabb.Min;
		glm::vec3 center = (aabb.Max + aabb.Min) * 0.5f;
		model = glm::translate(model, transform.Position + center * transform.Scale);
		//glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
		//glm::mat4 quatMatrix = glm::toMat4(rotation);
		//model *= quatMatrix;
		model = glm::scale(model, scale * transform.Scale);

		_aabbShader.setMat4("CamMat", camera.CameraMatrix());
		_aabbShader.setMat4("Model", model);

		_aabbVao.Bind();
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}