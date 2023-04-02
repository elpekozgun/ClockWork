#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "glad/glad.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"


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
	2- 50 of them could share the same mesh.

	designing a rendersystem to point a different memory block for the same data is insufficient,
	but for simplicity and to make it work initally ill just treat each renderable separately.

*/

namespace CW
{
	class CW_API RenderSystem : public ISystem
	{
	public:
		virtual void Update(float deltaTime) override;

	private:
		void Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera);
	};
}
