#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "glad/glad.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"
#include "Core/Input.h"
#include "Graphics/Model.h"

#include <map>
#include <execution>




/*
* TODO:
	Initial implementation.

	we will have a RenderableComponent that contains Mesh[]
	each mesh component will have
	{
		vertices,
		indices,
		textures,
		shader,
		vao
	}

	the drawbacks here are:
	1- 100 different models could share 5 shaders in total.
	2- 50 of them could share the same mesh. - done.

	designing a rendersystem to point a different memory block for the same data is insufficient,
	but for simplicity and to make it work initally ill just treat each renderable separately.

*/

namespace CW
{
	class CW_API RenderSystem : public ISystem
	{
	public:
		RenderSystem()
		{
			Name = "RenderSystem";
		}

		virtual void Update(float deltaTime) override;
		virtual void OnGui() override;

		void UpdateGetComponent(float deltaTime);

		std::vector<std::pair<TransformComponent, RenderableComponent>> RenderTuples;

	private:
		void Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera);
		void RenderInstanced(std::map<unsigned int, std::vector<glm::mat4>>& transformMap, CameraComponent& camera);
		glm::mat4 MatrixFromTransform(TransformComponent& component);
		glm::mat4 CameraMat(CameraComponent& camera);
		void SwitchState();

		bool IsInFrustum(glm::vec3& camPosition, glm::mat4& mvp, AABB& boundingbox, unsigned int& depth);
		
		std::unordered_map<unsigned int, MeshComponent> CachedMeshes;

		unsigned int maxOctreeDepth = 32;
		// for testing and profiling.	
		unsigned int MaxTri = 4000000;
		bool directionalLight = true;
		bool pagedInstanced = false;
		bool drawSkybox = true;
		bool instanced = false;
		bool frustum = false;
		unsigned int drawCall = 0;
		float cap;

		float normalScale = 1.0f;
		float metalScale = 1.0f;
		float smoothScale = 1.0f;
		float prevNormalScale = 1.0f;
		float prevMetalScale = 1.0f;
		float prevSmoothScale = 1.0f;
	};
}
