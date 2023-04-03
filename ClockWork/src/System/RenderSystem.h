#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "glad/glad.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"

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
		virtual void Update(float deltaTime) override;

		std::vector<std::pair<TransformComponent, RenderableComponent>> RenderTuples;

	private:
		void Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera);
		void RenderInstanced(std::map<unsigned int, std::vector<glm::mat4>>& transformMap, CameraComponent& camera);
		glm::mat4 MatrixFromTransform(TransformComponent& component);
		glm::mat4 CameraMat(CameraComponent& camera);

		// for testing and profiling.	
		unsigned int MaxTri = 4000000;
		bool pagedInstanced = false;
		bool instanced = false;
		bool frustum = false;
		unsigned int drawCall = 0;
		float cap;
		

	};
}
